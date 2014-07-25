#include "CameraImageContainerDeviceLinker.h"
#include "core/CameraDevice.h"
#include "core/SimpleCameraRotator.h"
#include "core/ImageBufferAnalyzer.h"
#include "core/RotationBufferAnalyzer.h"

#include "ui/CameraImageContainerUI.h"
#include "ui/CameraImageUI.h"
#include <tchar.h>

namespace camera {
  

class UnstableImageUpdate : public CameraImageContainerDeviceLinker::ImageUpdate {
public:

  virtual bool  OnImageUpdate(CameraImageBuffer* buffer, const CameraRotationPos& pos) {
    UnstableTempJudger temp_judger(std::numeric_limits<float>::max());
    float exception_temp;
    TempJudgeResult res = temp_judger.JudgeImageBuffer(buffer, &exception_temp);
    if (res == TEMP_NORMAL) {
      TCHAR sz_status[250];
      swprintf(sz_status, ARRAYSIZE(sz_status), _T("Update image, Camera Angle %d"), pos.GetAngle());
      status_show_->ShowStatusText(sz_status);
      return true;
    } else {
      TCHAR sz_status[250];
      switch (res) {
      case UNSTABLE_TEMP_EXCEPTION:
        swprintf(sz_status, ARRAYSIZE(sz_status), _T("the max temperature %.1f overflow"), exception_temp);
        break;
      case UNSTABLE_DELTA_EXCEPTION:
        swprintf(sz_status, ARRAYSIZE(sz_status), _T("delta overflow %.1f"), exception_temp);
        break;
      }
      status_show_->ShowErrorText(sz_status);
      return false;
    }
  }

};

class StableImageUpdate : public CameraImageContainerDeviceLinker::ImageUpdate {
public:
  virtual bool  OnImageUpdate(CameraImageBuffer* buffer, const CameraRotationPos& pos) override {
    rotation_buffer_analyzer_.AddImageBuffer(pos, buffer);
    return true;
  }

  virtual void  OnCompleteUpdate() override {
    CameraRotationPos pos;
    float exception_temp;
    TempJudgeResult res = rotation_buffer_analyzer_.JudgeImageBuffer(&pos, &exception_temp);
    if (res == TEMP_NORMAL) {
      TCHAR sz_status[250];
      swprintf(sz_status, ARRAYSIZE(sz_status), _T("Camera get stable temperature!"));
      status_show_->ShowStatusText(sz_status);
    } else {
      TCHAR sz_status[250];
      switch (res) {
      case STABLE_TEMP_EXCEPTION:
        swprintf(sz_status, ARRAYSIZE(sz_status), _T("stable temperature %.1f at %d overflow"), exception_temp, pos.GetAngle());
        break;
      case UNSTABLE_DELTA_EXCEPTION:
        swprintf(sz_status, ARRAYSIZE(sz_status), _T("stable delta overflow %.1f at %d"), exception_temp, pos.GetAngle());
        break;
      }
      status_show_->ShowErrorText(sz_status);
    }
  }
private:
  RotationBufferAnalyzer  rotation_buffer_analyzer_;
};

CameraImageContainerDeviceLinker::CameraImageContainerDeviceLinker()
    : device_status_(UNINITIALIZED),
      stable_sample_is_running_(false) {
}

CameraImageContainerDeviceLinker::~CameraImageContainerDeviceLinker() {
  camera_device_->RemoveConnectStatusObserver(this);
  if (device_status_ == CONNECTED || device_status_ == CONNECTING) {
    camera_device_->Disconnect();
  }
}

LPCTSTR CameraImageContainerDeviceLinker::GetIPAddr() const {
  return camera_device_->GetIPAddr().c_str();
}
void CameraImageContainerDeviceLinker::Init(LPCTSTR ip_addr, LPCTSTR name, CameraImageContainerUI* container_ui) {
  container_ui_.reset(container_ui);
  container_ui_->SetObserver(this);

  camera_device_.reset(new CameraDevice);
  camera_device_->AddConnectStatusObserver(this);
  camera_device_->SetName(name);
  camera_device_->SetIPAddr(ip_addr);
  camera_device_->Init();

  camera_rotator_.reset(new SimpleCameraRotator);

  container_ui_->EnableDisconnectButton(false);
  container_ui_->EnableConnectButton(false);
  container_ui_->EnableSampleButton(false);
  
  device_status_ = INITIALIZING;
}

void CameraImageContainerDeviceLinker::Connect() {
  container_ui_->ShowStatusHideError(true);
  if (device_status_ == INITIALIZED) {
    device_status_ = CONNECTING;    //the camera is connecting
    container_ui_->SetStatusText(_T("Connecting..."));
    camera_device_->Connect([this](IRCameraStatusCode code) {
      if (code == IRCAMERA_OK) {
      //  container_ui_->SetStatusText(_T("Connect successfully"));
        OnConnect();
      } else {
        device_status_ = INITIALIZED;
        container_ui_->SetStatusText(camera_device_->GetErrorString(code).c_str());
      }
    });
  }
}

void CameraImageContainerDeviceLinker::Disconnect() {
  container_ui_->ShowStatusHideError(true);
  if (device_status_ == CONNECTED) {
    device_status_ = DISCONNECTING;
    container_ui_->SetStatusText(_T("Disconnecting..."));
    camera_device_->Disconnect();
  }
}

void CameraImageContainerDeviceLinker::Sample() {
  if (device_status_ != CONNECTED)
    return;
  if (pending_unstable_update_.get() == NULL) {
    pending_unstable_update_.reset(new UnstableImageUpdate);
    pending_unstable_update_->Init(this);
    if (running_update_.get() == NULL) {
      running_update_.swap(pending_unstable_update_);
      BeginRotate();
    }
  }
}

void CameraImageContainerDeviceLinker::StableSample() {
  if (device_status_ != CONNECTED)
    return;
  if (pending_stable_update_.get() == NULL) {
    pending_stable_update_.reset(new StableImageUpdate);
    pending_stable_update_->Init(this);
    if (running_update_.get() == NULL) {
      running_update_.swap(pending_stable_update_);
      BeginRotate();
    }
  }
}
//when the camera has init completely
void  CameraImageContainerDeviceLinker::OnInitCamera() {
  device_status_ = INITIALIZED;
  container_ui_->EnableConnectButton(true);
  Connect();
}

//when connect to the camera successfully
void  CameraImageContainerDeviceLinker::OnConnect() {
  container_ui_->SetStatusText(_T("Connect successfully"));
  container_ui_->EnableConnectButton(false);
  container_ui_->EnableDisconnectButton(true);
  container_ui_->EnableSampleButton(true);
  device_status_ = CONNECTED;
}

//when the host has disconnect from the camera
void  CameraImageContainerDeviceLinker::OnDisconnect() {
  container_ui_->SetStatusText(_T("Disconnected"));
  container_ui_->EnableConnectButton(true);
  container_ui_->EnableDisconnectButton(false);
  container_ui_->EnableSampleButton(false);
  device_status_ = INITIALIZED;
  camera_rotator_->Reset();
  stable_sample_is_running_ = false;
}
//when the image has update
void  CameraImageContainerDeviceLinker::OnImageUpdate(CameraImageBuffer* buffer) {
  container_ui_->GetCameraImageUI()->UpdateImage(buffer);
  if (running_update_->OnImageUpdate(buffer, rotation_pos_))
    BeginRotate();
  else
    OnUpdateComplete();
}

void CameraImageContainerDeviceLinker::OnImageUpdateFail(IRCameraStatusCode code) {
  camera_device_->UpdateKelvinImage();
}

void CameraImageContainerDeviceLinker::OnConnectButtonClicked() {
  Connect();
}

void CameraImageContainerDeviceLinker::OnDisconnectButtonClicked() {
  Disconnect();
}
 

void CameraImageContainerDeviceLinker::OnSampleButtonClicked() {
  Sample();
}

void CameraImageContainerDeviceLinker::ShowStatusText(const TString& str) {
  container_ui_->SetStatusText(str.c_str());
  container_ui_->ShowStatusHideError(true);
}

void CameraImageContainerDeviceLinker::ShowErrorText(const TString& str) {
  container_ui_->SetErrorStatusText(str.c_str());
  container_ui_->ShowStatusHideError(false);
}

void  CameraImageContainerDeviceLinker::BeginRotate() {
  if (camera_rotator_->RotateNext(&rotation_pos_)) {
    camera_device_->UpdateKelvinImage();
  } else {
    running_update_->OnCompleteUpdate();
    OnUpdateComplete();
  }
}

void CameraImageContainerDeviceLinker::OnUpdateComplete() {
  camera_rotator_->Reset();
  running_update_.reset();
  if (pending_stable_update_)
    running_update_.swap(pending_stable_update_);
  else if (pending_unstable_update_)
    running_update_.swap(pending_unstable_update_);
  if (running_update_.get() != NULL)
    BeginRotate();
}
 


void CameraImageContainerDeviceLinker::ImageUpdate::Init(StateTextShow* text_show) {
  status_show_ = text_show;
}

}
