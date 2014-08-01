#include "CameraImageContainerDeviceLinker.h"
#include "core/CameraDevice.h"
#include "core/SimpleCameraRotator.h"
#include "core/ImageBufferAnalyzer.h"
#include "core/RotationBufferAnalyzer.h"

#include "ui/CameraImageContainerUI.h"
#include "ui/CameraImageUI.h"
#include "ui/CameraImageRealTimeUI.h"
#include <tchar.h>

namespace camera {

//////////////////////////////ImageUpdate///////////////////////////////////////
class ImageUpdate {
public:
  void Init(ImageUIOperator* text_show) {
    status_show_ = text_show;
  }
  virtual bool  OnImageUpdate(CameraImageBuffer* buffer, const CameraRotationPos&) = 0;
  virtual void  OnCompleteUpdate() {}
  virtual ~ImageUpdate() {}
protected:
  ImageUIOperator* status_show_;
}; 
  
//////////////////////////////UnstableImageUpdate//////////////////////////
class UnstableImageUpdate : public ImageUpdate {
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

  virtual void  OnCompleteUpdate() override {
    status_show_->ShowStatusText(_T("Image update completely"));
  }
};

////////////////////////StableImageUpdate////////////////////////////////////
class StableImageUpdate : public ImageUpdate {
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

//////////////////////////////////////////ImageUpdateSchedule//////////////////////////////////////
class ImageUpdateSchedule {
public:
  ImageUpdateSchedule(ImageUIOperator* ui_operator, DeviceOperator* device_operator, UpdateCompleteNotify* notify)
    : ui_operator_(ui_operator), device_operator_(device_operator), complete_notify_(notify) {
  }

  virtual void BeginSchedule() = 0;
  virtual void OnImageUpdate(CameraImageBuffer*) = 0;
  virtual ~ImageUpdateSchedule() {}
protected:
  ImageUIOperator*  ui_operator_;
  DeviceOperator*   device_operator_;
  UpdateCompleteNotify* complete_notify_;
};

//////////////////////////RealTimeImageUpdateSchedule/////////////////////////////////////////
class RealTimeImageUpdateSchedule : public ImageUpdateSchedule {
public:
  RealTimeImageUpdateSchedule(ImageUIOperator* ui_operator, DeviceOperator* device_operator, UpdateCompleteNotify* notify)
    : ImageUpdateSchedule(ui_operator, device_operator, notify) {
  }

  virtual void BeginSchedule() {
    device_operator_->UpdateKelvinImage();
  }
  virtual void OnImageUpdate(CameraImageBuffer* buffer) {
    ui_operator_->UpdateRealTimeImageUI(buffer);
    complete_notify_->OnUpdateComplete();
  }
};

/////////////////////////RotateImageUpdateSchedule/////////////////////////////////////////////
class RotateImageUpdateSchedule : public ImageUpdateSchedule {
public:
  RotateImageUpdateSchedule(ImageUIOperator* ui_operator, DeviceOperator* device_operator,
    UpdateCompleteNotify* notify, CameraRotator* rotate_runner)
    : ImageUpdateSchedule(ui_operator, device_operator, notify),rotate_runner_(rotate_runner) {
  } 

  virtual void BeginSchedule() {
    if (rotate_runner_->RotateNext(NULL)) {
      device_operator_->UpdateKelvinImage();
    } else {
      pending_image_update_->OnCompleteUpdate();
      complete_notify_->OnUpdateComplete();
    }
  }

  virtual void OnImageUpdate(CameraImageBuffer* buffer) {
    ui_operator_->UpdateImageUI(buffer);
    ui_operator_->UpdateRealTimeImageUI(buffer);
    if (pending_image_update_->OnImageUpdate(buffer, rotate_runner_->GetRotationPos()))
      BeginSchedule();
    else {      //the camera interrupt the update, so reset the rotator and notify the linker
      rotate_runner_->Reset();
      complete_notify_->OnUpdateComplete();
    }      
  }
protected:
  CameraRotator* rotate_runner_;
  std::unique_ptr<ImageUpdate>  pending_image_update_;
};

////////////////////StableImageUpdateSchedule////////////////////////////////////////////
class StableImageUpdateSchedule : public RotateImageUpdateSchedule {
public:
  StableImageUpdateSchedule(ImageUIOperator* ui_operator, DeviceOperator* device_operator,
       UpdateCompleteNotify* notify, CameraRotator* rotate_runner)
       : RotateImageUpdateSchedule(ui_operator, device_operator, notify, rotate_runner) {
    pending_image_update_.reset(new StableImageUpdate);
    pending_image_update_->Init(ui_operator_);
  }
};

//////////////////UnstableImageUpdateSchedule////////////////////////////
class UnstableImageUpdateSchedule : public RotateImageUpdateSchedule {
public:
  UnstableImageUpdateSchedule(ImageUIOperator* ui_operator, DeviceOperator* device_operator,
    UpdateCompleteNotify* notify, CameraRotator* rotate_runner)
    : RotateImageUpdateSchedule(ui_operator, device_operator, notify, rotate_runner) {
    pending_image_update_.reset(new UnstableImageUpdate);
    pending_image_update_->Init(ui_operator_);
  }
};

///////////////////////////CameraImageContainerDeviceLinker////////////////////////
CameraImageContainerDeviceLinker::CameraImageContainerDeviceLinker()
    : device_status_(UNINITIALIZED) {
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

  container_ui_->SetCameraName(name);
  container_ui_->EnableDisconnectButton(false);
  container_ui_->EnableConnectButton(false);
  container_ui_->EnableSampleButton(false);
  container_ui_->GetUnderlyingControl()->OnNotify += 
    DuiLib::MakeDelegate(this, &CameraImageContainerDeviceLinker::OnTimer);
  
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

void CameraImageContainerDeviceLinker::RealTimeSample() {
  if (device_status_ != CONNECTED)
    return;
  if (realtime_update_.get() == NULL) {
    realtime_update_.reset(
      new RealTimeImageUpdateSchedule(this, this, this));
    if (running_update_.get() == NULL) {
      running_update_.swap(realtime_update_);
      running_update_->BeginSchedule();
    }
  }
}

void CameraImageContainerDeviceLinker::Sample() {
  if (device_status_ != CONNECTED)
    return;
  if (pending_unstable_update_.get() == NULL) {
    pending_unstable_update_.reset(
      new UnstableImageUpdateSchedule(this, this, this, camera_rotator_.get()));
    if (running_update_.get() == NULL) {
      running_update_.swap(pending_unstable_update_);
      running_update_->BeginSchedule();
    }
  }
}

void CameraImageContainerDeviceLinker::StableSample() {
  if (device_status_ != CONNECTED)
    return;
  if (pending_stable_update_.get() == NULL) {
    pending_stable_update_.reset(
      new StableImageUpdateSchedule(this, this, this, camera_rotator_.get()));
    if (running_update_.get() == NULL) {
      running_update_.swap(pending_stable_update_);
      running_update_->BeginSchedule();
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

  BeginRealTimeSample();
}

//when the host has disconnect from the camera
void  CameraImageContainerDeviceLinker::OnDisconnect() {
  container_ui_->SetStatusText(_T("Disconnected"));
  container_ui_->EnableConnectButton(true);
  container_ui_->EnableDisconnectButton(false);
  container_ui_->EnableSampleButton(false);
  device_status_ = INITIALIZED;
  camera_rotator_->Reset();

  EndRealTimeSample();
}

void CameraImageContainerDeviceLinker::UpdateImageUI(CameraImageBuffer* buffer) {
  container_ui_->GetCameraImageUI()->UpdateImage(buffer);
}

void CameraImageContainerDeviceLinker::UpdateRealTimeImageUI(CameraImageBuffer* buffer) {
  container_ui_->GetCamreaImageRealTimeUI()->UpdateImage(buffer);
}

//when the image has update
void  CameraImageContainerDeviceLinker::OnImageUpdate(CameraImageBuffer* buffer) {
  running_update_->OnImageUpdate(buffer);
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

void CameraImageContainerDeviceLinker::UpdateKelvinImage() {
  camera_device_->UpdateKelvinImage();
}
 
void CameraImageContainerDeviceLinker::OnUpdateComplete() {
  running_update_.reset();
  if (pending_stable_update_)
    running_update_.swap(pending_stable_update_);
  else if (pending_unstable_update_)
    running_update_.swap(pending_unstable_update_);
  else if (realtime_update_)
    running_update_.swap(realtime_update_);
  if (running_update_.get() != NULL)
    running_update_->BeginSchedule();
}
 
const static int kRealTimeSampleTimerID = 1;
void  CameraImageContainerDeviceLinker::BeginRealTimeSample() {
  DuiLib::CControlUI* control = container_ui_->GetUnderlyingControl();
  control->GetManager()->SetTimer(control, kRealTimeSampleTimerID, 200);  //sample per 1 second
}

void  CameraImageContainerDeviceLinker::EndRealTimeSample() {
  DuiLib::CControlUI* control = container_ui_->GetUnderlyingControl();
  control->GetManager()->KillTimer(control, kRealTimeSampleTimerID); 
}

bool CameraImageContainerDeviceLinker::OnTimer(void* param) {
  DuiLib::TNotifyUI* notify = static_cast<DuiLib::TNotifyUI*>(param);
  if (notify->sType == _T("timer")) {
    RealTimeSample();
    return true;
  }
  return false;
}
}
