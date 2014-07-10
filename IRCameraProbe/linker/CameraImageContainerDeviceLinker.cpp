#include "CameraImageContainerDeviceLinker.h"
#include "core/CameraDevice.h"
#include "core/SimpleCameraRotator.h"
#include "core/ImageBufferAnalyzer.h"
#include "core/RotationBufferAnalyzer.h"

#include "ui/CameraImageContainerUI.h"
#include "ui/CameraImageUI.h"
#include <tchar.h>

namespace camera {
  
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
  container_ui_->ShowStatusHideError(true);
  if (device_status_ != CONNECTED)
    return;
  //rotate all of the angle to get the around image
  SampleNewPos();
}

void CameraImageContainerDeviceLinker::StableSample() {
  container_ui_->ShowStatusHideError(true);
  if (device_status_ != CONNECTED)
    return;
  stable_buffer_analyzer_.reset(new RotationBufferAnalyzer);
  StableSampleNewPos();
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
}
//when the image has update
void  CameraImageContainerDeviceLinker::OnImageUpdate(CameraImageBuffer* buffer) {
  ImageBufferUpdate(buffer);
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

void CameraImageContainerDeviceLinker::ImageBufferUpdate(CameraImageBuffer* buffer) {
  container_ui_->GetCameraImageUI()->UpdateImage(buffer);
  UnstableTempJudger temp_judger(std::numeric_limits<float>::max());
  float exception_temp;
  TempJudgeResult res = temp_judger.JudgeImageBuffer(buffer, &exception_temp);
  if (res == TEMP_NORMAL) {
    TCHAR sz_status[250];
    swprintf(sz_status, ARRAYSIZE(sz_status), _T("Camera Angle %d: Update image"), rotation_pos_.GetAngle());
    container_ui_->SetStatusText(sz_status);
    SampleNewPos();
  }
  else {
    TCHAR sz_status[250];
    switch (res) {
    case UNSTABLE_TEMP_EXCEPTION: 
      swprintf(sz_status, ARRAYSIZE(sz_status), _T("the max temperature %.1f overflow"), exception_temp);
      break;
    case UNSTABLE_DELTA_EXCEPTION:
      swprintf(sz_status, ARRAYSIZE(sz_status), _T("delta overflow %.1f"), exception_temp);
      break;
    }
    camera_rotator_->Reset();
    container_ui_->SetErrorStatusText(sz_status);
    container_ui_->ShowStatusHideError(false);
  }
}

void CameraImageContainerDeviceLinker::SampleNewPos()
{
  if (camera_rotator_->RotateNext(&rotation_pos_)) {
    camera_device_->UpdateKelvinImage();
  }
}

void CameraImageContainerDeviceLinker::StableSampleNewPos() {
  if (camera_rotator_->RotateNext(&rotation_pos_)) {
    camera_device_->UpdateKelvinImage();
  }
}

void CameraImageContainerDeviceLinker::StableSampleImageBufferUpdate(CameraImageBuffer* buffer) {
  UnstableTempJudger temp_judger(std::numeric_limits<float>::max());
  stable_buffer_analyzer_->AddImageBuffer(rotation_pos_, buffer);
  if (camera_rotator_->RotateNext(&rotation_pos_)) {
    camera_device_->UpdateKelvinImage();
  } else {  //rotation end, the around image has got
    CameraRotationPos pos;
    float exception_temp;
    TempJudgeResult res = stable_buffer_analyzer_->JudgeImageBuffer(&pos, &exception_temp);
    if (res == TEMP_NORMAL) {
      TCHAR sz_status[250];
      swprintf(sz_status, ARRAYSIZE(sz_status), _T("Camera get stable temperature!"));
      container_ui_->SetStatusText(sz_status);
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
      camera_rotator_->Reset();
      container_ui_->SetErrorStatusText(sz_status);
      container_ui_->ShowStatusHideError(false);
    }
  }
}

}
