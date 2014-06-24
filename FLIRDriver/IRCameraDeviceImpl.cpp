#include "stdafx.h"
#include "IRCameraDeviceImpl.h"
#include "CameraImageBufferImp.h"
#include "lvcamctrl.h"
#include "FakeDialog.h"
#include <assert.h>

namespace {
  const short PROP_IMAGE_WIDTH = 66;		// Image horizontal line width in pixels
  const short PROP_IMAGE_HEIGHT = 67;		// Image height in lines (or rows)
}



//the event handler for the camera event, such as connect and disconnect
class InternalEventHandler : public FakeDialogEventObserver {
public:
  InternalEventHandler(IRCameraDeviceImpl* info);
  virtual void OnEvent(int event_type);
private:
  IRCameraDeviceImpl* camera_info;
};

///////////////the implement of InternalEventHandler//////////////////////
InternalEventHandler::InternalEventHandler(IRCameraDeviceImpl* info) :camera_info(info) {}

void InternalEventHandler::OnEvent(int event_type) {
  switch (event_type) {
  case IRCAMERA_CONNECTED_EVENT:
    camera_info->camera_status_ = IRCAMERA_CONNECTED;
    break;
  case IRCAMERA_DISCONNECTED_EVENT:
    camera_info->camera_status_ = IRCAMERA_DISCONNECTED;
    break;
  case IRCAMERA_CONNECTBROKEN_EVENT:
    camera_info->camera_status_ = IRCAMERA_BROKEN_CONNECTE;
    break;
  case IRCAMERA_RECONNECTED_EVENT:
    camera_info->camera_status_ = IRCAMERA_RECONNECTED;
    break;
  }
  if (camera_info && camera_info->event_handler_)
    camera_info->event_handler_->OnEvent(static_cast<IRCameraEvent>(event_type));
}

/////////////the implement of IRCameraDevice/////////
IRCameraDeviceImpl::IRCameraDeviceImpl() {
  image_width_ = image_height_ = 0;
  camera_status_ = IRCAMERA_DISCONNECTED;
  internal_event_handler_ = new InternalEventHandler(this);
  event_handler_ = NULL;

  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  fake_dlg_ = new FakeDialog;
  BOOL res = fake_dlg_->Create(FakeDialog::IDD);
  fake_dlg_->SetEventHandler(internal_event_handler_);
  assert(res);

  camera_ = &fake_dlg_->camera_control;
}

IRCameraDeviceImpl::~IRCameraDeviceImpl() {
  delete internal_event_handler_;
  delete fake_dlg_;
}

IRCameraStatusCode  IRCameraDeviceImpl::Connect(const TString& ip_addr) {
  const static short  CAM_A300 = 11;
  const static short  DEVICE_ETHERNET_16 = 6;	// 16-bit images over Ethernet
  const static short  CAM_INTF_TCPIP = 2;
//  TCHAR  ipaddr[] = _T("169.254.141.78");  //the IP address is the camera address, but the camera can use dynamic IP, so the address need change
  short code = camera_->Connect(CAM_A300, 0, DEVICE_ETHERNET_16, CAM_INTF_TCPIP, ip_addr.c_str());
  return static_cast<IRCameraStatusCode>(code);
}

IRCameraStatusCode  IRCameraDeviceImpl::Disconnect() {
  short code = camera_->Disconnect();
  return static_cast<IRCameraStatusCode>(code);
}

TString IRCameraDeviceImpl::GetErrorString(IRCameraStatusCode code) {
  CString str_res = camera_->GetError(code);
  return str_res.GetString();
}


int  IRCameraDeviceImpl::GetImageWidth() {
  if (image_width_ == 0) {
    VARIANT va = camera_->GetCameraProperty(PROP_IMAGE_WIDTH);
    image_width_ = va.iVal;
  }
  return image_width_;
}

int  IRCameraDeviceImpl::GetImageHeight() {
  if (image_height_ == 0) {
    VARIANT va = camera_->GetCameraProperty(PROP_IMAGE_HEIGHT);
    image_height_ = va.iVal;
  }
  return  image_height_;
}

IRCameraStatusCode IRCameraDeviceImpl::GetKelvinImage(CameraImageBuffer** buffer) {
  if (GetStatus() != IRCAMERA_CONNECTED)
    return IRCAMERA_NOTPRESENT_ERROR;
  const static short KelvinImageType = 3;
  VARIANT va = camera_->GetImage(20 + KelvinImageType);
  int ret = -1;
  if (va.vt == VT_I2) {
    ret = va.iVal;
  }
  else if (va.vt == VT_I4) {
    HGLOBAL h = (HGLOBAL)va.lVal;
    *buffer = new CameraImageBufferImp(h, GetImageWidth(), GetImageHeight());
    ret = IRCAMERA_OK;
  }
  VariantClear(&va);
  return static_cast<IRCameraStatusCode>(ret);
}

void IRCameraDeviceImpl::RegisterEventHandler(IRCameraEventHandler* handler) {
  event_handler_ = handler;
}

IRCameraDeviceStatus IRCameraDeviceImpl::GetStatus() {
  return camera_status_;
}