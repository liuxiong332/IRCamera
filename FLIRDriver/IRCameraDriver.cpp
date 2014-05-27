#include "stdafx.h"
#include "IRCameraDriver.h"
#include "lvcamctrl.h"
#include "FakeDialog.h"
#include <assert.h>

namespace {
  const short PROP_IMAGE_WIDTH = 66;		// Image horizontal line width in pixels
  const short PROP_IMAGE_HEIGHT = 67;		// Image height in lines (or rows)
}


struct IRCameraInfo {
  FakeDialog  fake_dlg;
  CLvcamctrl*  camera;
  int image_width;
  int image_height;
};
 
IRCameraInfo* IRCameraCreate() {
  IRCameraInfo* info = new IRCameraInfo;
  info->image_height = info->image_width = 0;

  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  BOOL res = info->fake_dlg.Create(FakeDialog::IDD);
  assert(res);
  info->camera = &info->fake_dlg.camera_control;
  return info;
}

void IRCameraDestroy(IRCameraInfo* info) {
  delete info;
}
IRCameraStatusCode  IRCameraConnect(IRCameraInfo* info) {
  const short  CAM_A300 = 11;
  const short  DEVICE_ETHERNET_16 = 6;	// 16-bit images over Ethernet
  const short  CAM_INTF_TCPIP = 2;
  TCHAR  ipaddr[] = _T("169.254.165.211");  //the IP address is the camera address, but the camera can use dynamic IP, so the address need change
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

IRCameraStatusCode IRCameraGetKelvinImage(IRCameraInfo* info, float* temp_image) {
  const static short KelvinImageType = 3;
  VARIANT va = info->camera->GetImage(20 + KelvinImageType);
  int ret = -1;
  if (va.vt == VT_I2) {
    ret = va.iVal;
  }
  else if (va.vt == VT_I4) {
    HGLOBAL h = (HGLOBAL)va.lVal;
    size_t  image_bytes = IRCameraGetImageWidth(info)*IRCameraGetImageHeight(info) * sizeof(float);
    void* src = GlobalLock(h);
    memcpy_s(temp_image, image_bytes, src, image_bytes);
    GlobalUnlock(h);
    GlobalFree(h);
    ret = IRCamera_OK;
  }
  VariantClear(&va);
  return ret;
}

void IRCameraRegisterEventHandler(IRCameraInfo* info, IRCameraEventHandler handler) {
  info->fake_dlg.SetEventHandler(handler);
}