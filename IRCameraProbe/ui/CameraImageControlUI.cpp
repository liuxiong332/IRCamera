#include "CameraImageControlUI.h"
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
CameraImageControlUI::CameraImageControlUI(CameraManage* manager) :
  camera_manage(manager)   {
  min_temp = kKelvinTransform + 20;
  threshold_temp_ = max_temp = kKelvinTransform + 100;
  is_need_update_ = false;
   
  OnNotify += MakeDelegate(this, &CameraImageControlUI::ImageCtrlTimerProc);
  camera_manage->AddConnectStatusObserver(this );
//  InitBitmapHeader();
}

bool CameraImageControlUI::ImageCtrlTimerProc(void* param) {
  DuiLib::TNotifyUI*  notify = reinterpret_cast<DuiLib::TNotifyUI*>(param);
  if (notify->sType == _T("timer")) {
    camera_manage->UpdateKelvinImage(this);
    return true;
  }
  return false;
}

void CameraImageControlUI::SetThresholdTemp(float threshold_temp) {
  threshold_temp_ = threshold_temp + kKelvinTransform;
}

CameraImageControlUI::~CameraImageControlUI() {
  OnDisconnect();
  camera_manage->RemoveConnectStatusObserver(this);
}

// void CameraImageControlUI::UpdateTempLabel(float min_temp, float max_temp) {
//   const int kBufferLen = 50;
//   TCHAR sz_temp[kBufferLen];
//   if (min_label_) {
//     StringCchPrintf(sz_temp, kBufferLen, _T("%.1f C"), min_temp-kKelvinTransform);
//     min_label_->SetText(sz_temp);
//   }
//   if (max_label_) {
//     StringCchPrintf(sz_temp, kBufferLen,  _T("%.1f C"), max_temp-kKelvinTransform);
//     max_label_->SetText(sz_temp);
//   }
// }

void CameraImageControlUI::OnConnect() {
  img_width = camera_manage->GetImageWidth();
  align_width = (img_width + 3)&~3;
  img_height = camera_manage->GetImageHeight();
  bitmap_.Init(img_width, img_height);
}
  
void CameraImageControlUI::OnImageUpdate() {
  is_need_update_ = true;
  this->Invalidate();
}

void CameraImageControlUI::OnDisconnect() {
}

int CameraImageControlUI::GetMidColor(int begin, int end, int i) {
  const int count = bitmap_.GetPaletteLen() - 1;
  return (end*i + begin*(count - i)) / count;
}

// void CameraImageControlUI::InitPalette() {
//   //Init the palette
//   int red_begin = 0x33, red_end = 0xf6;
//   int green_begin = 0x9c, green_end = 0xaa;
//   int blue_begin = 0x29, blue_end = 0x9f;
//  
//   int palette_count = bitmap_.GetPaletteLen();
//   RGBQUAD * palette_buffer = bitmap_.GetPalettePtr();
//   RGBQUAD * ui_palette_buffer = NULL;
//   if (temp_color_ui_)
//     ui_palette_buffer = temp_color_ui_->bitmap_.GetPalettePtr();
// 
//   for (int i = 0; i < palette_count-1; ++i) {
//     RGBQUAD color = { 
//       GetMidColor(red_begin,red_end,i), 
//       GetMidColor(green_begin, green_end, i),
//       GetMidColor(blue_begin,blue_end,i), 0 };
//     palette_buffer[i] = color;
//     if (ui_palette_buffer)
//       ui_palette_buffer[i] = color;
//   }
//   RGBQUAD red_color = { 0xFF, 0, 0, 0 };
//   palette_buffer[palette_count - 1] = red_color;
//   if (temp_color_ui_)
//     temp_color_ui_->Invalidate();
// }

LPCTSTR CameraImageControlUI::GetClass() const {
  return _T("CameraImageUI");
}

void CameraImageControlUI::SetBuffer(float* val, int val_count) {

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

void CameraImageControlUI::PaintStatusImage(HDC hDC) {
  if (camera_manage->GetStatus() != CameraManage::CONNECTED || !is_need_update_)
    return;
  bitmap_.Paint(hDC, m_rcItem);
}

