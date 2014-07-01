#include "CameraImageContainerDeviceLinker.h"
#include "core/CameraDevice.h"
#include "ui/CameraImageContainerUI.h"
#include "ui/CameraImageUI.h"
#include <tchar.h>

namespace camera {
  
CameraImageContainerDeviceLinker::CameraImageContainerDeviceLinker()
    : device_status_(UNINITIALIZED) {
}

LPCTSTR CameraImageContainerDeviceLinker::GetIPAddr() const {
  return camera_device_->GetIPAddr().c_str();
}
void CameraImageContainerDeviceLinker::Init(LPCTSTR name, LPCTSTR ip_addr, CameraImageContainerUI* container_ui) {
  container_ui_.reset(container_ui);
  container_ui_->SetObserver(this);

  camera_device_.reset(new CameraDevice);
  camera_device_->AddConnectStatusObserver(this);
  camera_device_->SetName(name);
  camera_device_->SetIPAddr(ip_addr);
  camera_device_->Init();

  container_ui_->EnableDisconnectButton(false);
  container_ui_->EnableConnectButton(false);
  container_ui_->EnableSampleButton(false);
  
  device_status_ = INITIALIZING;
}

void CameraImageContainerDeviceLinker::Connect() {
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
  if (device_status_ == CONNECTED) {
    device_status_ = DISCONNECTING;
    container_ui_->SetStatusText(_T("Disconnecting..."));
    camera_device_->Disconnect();
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
}
//when the image has update
void  CameraImageContainerDeviceLinker::OnImageUpdate() {
}

void CameraImageContainerDeviceLinker::OnConnectButtonClicked() {
  Connect();
}

void CameraImageContainerDeviceLinker::OnDisconnectButtonClicked() {
  Disconnect();
}

void CameraImageContainerDeviceLinker::OnSampleButtonClicked() {
  if (device_status_ != CONNECTED)
    return;
  camera_device_->UpdateKelvinImage(
    std::bind(&CameraImageContainerDeviceLinker::ImageBufferUpdate, this, std::placeholders::_1));
}

void CameraImageContainerDeviceLinker::ImageBufferUpdate(CameraImageBuffer* buffer) {
  container_ui_->GetCameraImageUI()->UpdateImage(buffer);
  container_ui_->SetStatusText(_T("Update image"));
}
}
