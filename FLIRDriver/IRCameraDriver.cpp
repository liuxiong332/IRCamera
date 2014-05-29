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
class InternalEventHandler : public FakeDialog::EventObserver {
public:
  InternalEventHandler(IRCameraInfo* info);
  virtual void OnEvent(int event_type);
private:
  IRCameraInfo* camera_info;
};

struct IRCameraInfo {
  FakeDialog  fake_dlg;
  CLvcamctrl*  camera;
  int camera_status;

  int image_width;
  int image_height;
  InternalEventHandler*  internal_handler;
  IRCameraEventHandler  event_handler;
  void*   handler_args;
};

///////////////the implement of InternalEventHandler//////////////////////
InternalEventHandler::InternalEventHandler(IRCameraInfo* info) :camera_info(info) {}

void InternalEventHandler::OnEvent(int event_type) {
  switch (event_type) {
  case IRCAMERA_CONNECTED_EVENT:
    camera_info->camera_status = IRCAMERA_CONNECTED;
    break;
  case IRCAMERA_DISCONNECTED_EVENT:
    camera_info->camera_status = IRCAMERA_DISCONNECTED;
    break;
  case IRCAMERA_CONNECTBROKEN_EVENT:
    camera_info->camera_status = IRCAMERA_BROKEN_CONNECTE;
    break;
  case IRCAMERA_RECONNECTED_EVENT:
    camera_info->camera_status = IRCAMERA_RECONNECTED;
    break;
  }
  if (camera_info && camera_info->event_handler)
    camera_info->event_handler(event_type, camera_info->handler_args);
}

/////////////the implement of IRCameraInfo/////////
IRCameraInfo* IRCameraCreate() {
  IRCameraInfo* info = new IRCameraInfo;
  info->image_height = info->image_width = 0;
  info->camera_status = IRCAMERA_DISCONNECTED;
  info->internal_handler = new InternalEventHandler(info);
  info->event_handler = NULL;

  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  BOOL res = info->fake_dlg.Create(FakeDialog::IDD);
  info->fake_dlg.SetEventHandler(info->internal_handler);
  assert(res);

  info->camera = &info->fake_dlg.camera_control;
  return info;
}

void IRCameraDestroy(IRCameraInfo* info) {
  delete info->internal_handler;
  delete info;
}
IRCameraStatusCode  IRCameraConnect(IRCameraInfo* info) {
  const short  CAM_A300 = 11;
  const short  DEVICE_ETHERNET_16 = 6;	// 16-bit images over Ethernet
  const short  CAM_INTF_TCPIP = 2;
  TCHAR  ipaddr[] = _T("169.254.18.63");  //the IP address is the camera address, but the camera can use dynamic IP, so the address need change
  return info->camera->Connect(CAM_A300, 0, DEVICE_ETHERNET_16, CAM_INTF_TCPIP, ipaddr);
}

IRCameraStatusCode  IRCameraDisconnect(IRCameraInfo* info) {
  return info->camera->Disconnect();
}

void IRCameraGetError(IRCameraInfo* info, IRCameraStatusCode code, LPTSTR error_str, int str_len) {
  CString str_res = info->camera->GetError(code);
  _tcscpy_s(error_str, str_len, str_res.GetString());
}

int  IRCameraGetImageWidth(IRCameraInfo* info) {
  if (info->image_width == 0) {
    VARIANT va = info->camera->GetCameraProperty(PROP_IMAGE_WIDTH);
    info->image_width = va.iVal;
  }
  return info->image_width;
}

int  IRCameraGetImageHeight(IRCameraInfo* info) {
  if (info->image_height==0) {
    VARIANT va = info->camera->GetCameraProperty(PROP_IMAGE_HEIGHT);
    info->image_height = va.iVal;
  }
  return info->image_height;
}

IRCameraStatusCode IRCameraGetKelvinImage(IRCameraInfo* info, IRCameraImageFilling* img_filling) {
  if (IRCameraGetStatus(info) != IRCAMERA_CONNECTED)
    return IRCAMERA_NOTPRESENT_ERROR;
  const static short KelvinImageType = 3;
  VARIANT va = info->camera->GetImage(20 + KelvinImageType);
  int ret = -1;
  if (va.vt == VT_I2) {
    ret = va.iVal;
  }
  else if (va.vt == VT_I4) {
    HGLOBAL h = (HGLOBAL)va.lVal;
    size_t  image_bytes = IRCameraGetImageWidth(info)*IRCameraGetImageHeight(info) * sizeof(float);
    float* src = (float*)GlobalLock(h);
  //  memcpy_s(temp_image, image_bytes, src, image_bytes);
    img_filling->set_buffer(img_filling->args, (float*)src, 
      info->image_height*info->image_width);

    GlobalUnlock(h);
    GlobalFree(h);
    ret = IRCAMERA_OK;
  }
  VariantClear(&va);
  return ret;
}

void IRCameraRegisterEventHandler(IRCameraInfo* info, IRCameraEventHandler handler, void* args) {
  info->event_handler = handler;
  info->handler_args = args;
}
int IRCameraGetStatus(IRCameraInfo* info) {
  return info->camera_status;
}