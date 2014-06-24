#include "CameraImageUI.h"
#include "IRCameraDevice.h"

#include <algorithm>
#include <memory>
#include <windows.h>
#include <strsafe.h>

namespace {
  const static float kKelvinTransform = 273.15f;
}

class TemperatureColorTableUI;

//////////////////////CameraImageControlUI/////////////////////////
CameraImageUI::CameraImageUI(camera::CameraDevice* manager)
    : camera_manage(manager)   {
  min_temp = kKelvinTransform + 20;
  threshold_temp_ = max_temp = kKelvinTransform + 100;
  is_need_update_ = false;
   
  OnNotify += MakeDelegate(this, &CameraImageUI::ImageCtrlTimerProc);
  camera_manage->AddConnectStatusObserver(this );
//  InitBitmapHeader();
}

bool CameraImageUI::ImageCtrlTimerProc(void* param) {
  DuiLib::TNotifyUI*  notify = reinterpret_cast<DuiLib::TNotifyUI*>(param);
  if (notify->sType == _T("timer")) {
    camera_manage->UpdateKelvinImage(this);
    return true;
  }
  return false;
}
 

CameraImageUI::~CameraImageUI() {
  OnDisconnect();
  camera_manage->RemoveConnectStatusObserver(this);
}
 
LPCTSTR CameraImageUI::GetClass() const {
  return _T("CameraImageUI");
}

void CameraImageUI::SetBuffer(float* val, int val_count) {

  float min_temp = (float)INT_MAX, max_temp = (float)INT_MIN;
  for (int i = 0; i < val_count; ++i) {
    min_temp = std::min(min_temp, val[i]);
    max_temp = std::max(max_temp, val[i]);
  }
//  UpdateTempLabel(min_temp, max_temp);

  max_temp = std::min(max_temp, threshold_temp_);
  float span = max_temp - min_temp;
  int len = 0;
  //protect the image buffer from the synchronous access
//  WaitForSingleObject(buffer_mutex_, INFINITE);
  BYTE* buffer = bitmap_.GetBitBuffer();
  for (int i = 0; i < img_height; ++i) {
    for (int j = 0; j < img_width; ++j) {
      float temp = std::min(val[len], max_temp);
      buffer[j] = static_cast<BYTE>((temp - min_temp) * 255 / span);
      ++len;
    }
    buffer += align_width;
  }
//  ReleaseMutex(buffer_mutex_);
}

void CameraImageUI::PaintStatusImage(HDC hDC) {
  if (camera_manage->GetStatus() != camera::CameraDevice::CONNECTED || !is_need_update_)
    return;
  bitmap_.Paint(hDC, m_rcItem);
}

