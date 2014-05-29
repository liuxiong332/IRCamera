#include "stdafx.h"
#include "IRCameraDriver.h"
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
  InternalEventHandler(IRCameraDevice* info);
  virtual void OnEvent(int event_type);
private:
  IRCameraDevice* camera_info;
};

///////////////the implement of InternalEventHandler//////////////////////
InternalEventHandler::InternalEventHandler(IRCameraDevice* info) :camera_info(info) {}

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
    camera_info->event_handler_->OnEvent( static_cast<IRCameraEvent>(event_type));
}

/////////////the implement of IRCameraDevice/////////
IRCameraDevice::IRCameraDevice() {
  image_width_ = image_height_ = 0;
  camera_status_ = IRCAMERA_DISCONNECTED;
  internal_event_handler_ = new InternalEventHandler(this);
  event_handler_ = NULL;

  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  fake_dlg_ = new FakeDialog;
  BOOL res = fake_dlg_->Create(FakeDialog::IDD);
  fake_dlg_->SetEventHandler( internal_event_handler_ );
  assert(res);

  camera_ = &fake_dlg_->camera_control;
}

IRCameraDevice::~IRCameraDevice() {
  delete internal_event_handler_;
  delete fake_dlg_;
}

IRCameraStatusCode  IRCameraDevice::Connect() {
  const static short  CAM_A300 = 11;
  const static short  DEVICE_ETHERNET_16 = 6;	// 16-bit images over Ethernet
  const static short  CAM_INTF_TCPIP = 2;
  TCHAR  ipaddr[] = _T("169.254.18.63");  //the IP address is the camera address, but the camera can use dynamic IP, so the address need change
  short code = camera_->Connect(CAM_A300, 0, DEVICE_ETHERNET_16, CAM_INTF_TCPIP, ipaddr);
  return static_cast<IRCameraStatusCode>(code);
}

IRCameraStatusCode  IRCameraDevice::Disconnect() {
  short code = camera_->Disconnect();
  return static_cast<IRCameraStatusCode>(code);
}

TString IRCameraDevice::GetErrorString(IRCameraStatusCode code) {
  CString str_res = camera_->GetError(code);
  return str_res.GetString();
}

void IRCameraDevice::SetImageWidth(int width) {
//  info->camera->SetCameraProperty(PROP_IMAGE_WIDTH, )
}
void IRCameraDevice::SetImageHeight(int height) {

}

int  IRCameraDevice::GetImageWidth() {
  if (image_width_ == 0) {
    VARIANT va = camera_->GetCameraProperty(PROP_IMAGE_WIDTH);
    image_width_ = va.iVal;
  }
  return image_width_;
}

int  IRCameraDevice::GetImageHeight() {
  if (image_height_ == 0) {
    VARIANT va = camera_->GetCameraProperty(PROP_IMAGE_HEIGHT);
    image_height_ = va.iVal;
  }
  return  image_height_;
}

IRCameraStatusCode IRCameraDevice::GetKelvinImage(IRCameraImageFilling* img_filling) {
  if ( GetStatus() != IRCAMERA_CONNECTED)
    return IRCAMERA_NOTPRESENT_ERROR;
  const static short KelvinImageType = 3;
  VARIANT va = camera_->GetImage(20 + KelvinImageType);
  int ret = -1;
  if (va.vt == VT_I2) {
    ret = va.iVal;
  }
  else if (va.vt == VT_I4) {
    HGLOBAL h = (HGLOBAL)va.lVal;
    size_t  image_bytes = GetImageWidth()*GetImageHeight() * sizeof(float);
    float* src = (float*)GlobalLock(h);
  //  memcpy_s(temp_image, image_bytes, src, image_bytes);
    img_filling->SetBuffer((float*)src, image_width_*image_height_);

    GlobalUnlock(h);
    GlobalFree(h);
    ret = IRCAMERA_OK;
  }
  VariantClear(&va);
  return static_cast<IRCameraStatusCode>(ret);
}

void IRCameraDevice::RegisterEventHandler(IRCameraEventHandler* handler) {
  event_handler_ = handler;
}

IRCameraDeviceStatus IRCameraDevice::GetStatus() {
  return camera_status_;
}