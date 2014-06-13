#include "CameraImageControl.h"
#include "IRCameraDevice.h"

#include <algorithm>
#include <memory>
#include <windows.h>
#include <strsafe.h>

namespace {
  const static float kKelvinTransform = 273.15f;
}

DIBitmap::DIBitmap() :bitmap_header_(NULL),bitmap_buffer_(NULL),bitmap_palette_(NULL) {}
DIBitmap::~DIBitmap() {
  if (bitmap_header_)
    free(bitmap_header_);
}
void DIBitmap::Init(int width, int height) {
  if (bitmap_header_)
    return;
  img_width_ = width;
  img_height_ = height;

  int palette_size = PALETTE_COUNT * sizeof(RGBQUAD);  //the size of palette
  int align_width = (width + 3)&~3;   //4 byte align
  int img_size = sizeof(BITMAPINFOHEADER);
  img_size += palette_size;
  img_size += align_width * height;

  bitmap_header_ = (BITMAPINFOHEADER*)malloc(img_size);
  bitmap_buffer_ = (BYTE*)bitmap_header_ + sizeof(BITMAPINFOHEADER)+palette_size;

  bitmap_header_->biSize = sizeof(BITMAPINFOHEADER);
  bitmap_header_->biWidth = width;
  bitmap_palette_ = (RGBQUAD*)(((LPBITMAPINFO)bitmap_header_)->bmiColors);
  // Specifies the height of the bitmap, in pixels.
  // If biHeight is positive, the bitmap is a bottom-up DIB and
  // its origin is the lower-left corner. If biHeight is negative,
  // the bitmap is a top-down DIB and its origin is the upper-left corner.
  bitmap_header_->biHeight = -height;
  bitmap_header_->biPlanes = 1;
  bitmap_header_->biBitCount = 8;
  bitmap_header_->biCompression = BI_RGB;
  bitmap_header_->biSizeImage = align_width * height;
  bitmap_header_->biXPelsPerMeter = 0;
  bitmap_header_->biYPelsPerMeter = 0;
  bitmap_header_->biClrUsed = 256;
  bitmap_header_->biClrImportant = 0;

}
BITMAPINFO* DIBitmap::GetInfo() {
  return reinterpret_cast<BITMAPINFO*>(bitmap_header_);
}

void DIBitmap::SetPalette(int index, const RGBQUAD& color) {
  bitmap_palette_[index] = color;
}
int DIBitmap::GetPaletteLen() const {
  return PALETTE_COUNT;
}
RGBQUAD*  DIBitmap::GetPalettePtr() {
  return bitmap_palette_;
}

BYTE* DIBitmap::GetBitBuffer(){
  return bitmap_buffer_;
}
void  DIBitmap::Paint(HDC hdc, RECT rcItem) {
  int old_mode = SetStretchBltMode(hdc, COLORONCOLOR);
  ::StretchDIBits(hdc, rcItem.left, rcItem.top, rcItem.right - rcItem.left, 
    rcItem.bottom - rcItem.top,
    0, 0, img_width_, img_height_, GetBitBuffer(), GetInfo(),
    DIB_RGB_COLORS, SRCCOPY);
  SetStretchBltMode(hdc, old_mode);
}

class TemperatureColorTableUI;

//////////////////////CameraImageControlUI/////////////////////////
CameraImageControlUI::CameraImageControlUI(CameraManage* manager) :
  camera_manage(manager)   {
  min_temp = kKelvinTransform + 20;
  threshold_temp_ = max_temp = kKelvinTransform + 100;
  min_label_ = max_label_ = NULL;
  temp_color_ui_ = NULL;
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

void CameraImageControlUI::BindTempLabel(DuiLib::CLabelUI* min_label, 
  DuiLib::CLabelUI* max_label) {
  min_label_ = min_label;
  max_label_ = max_label;
}

CameraImageControlUI::~CameraImageControlUI() {
  OnDisconnect();
  camera_manage->RemoveConnectStatusObserver(this);
}

void CameraImageControlUI::UpdateTempLabel(float min_temp, float max_temp) {
  const int kBufferLen = 50;
  TCHAR sz_temp[kBufferLen];
  if (min_label_) {
    StringCchPrintf(sz_temp, kBufferLen, _T("%.1f C"), min_temp-kKelvinTransform);
    min_label_->SetText(sz_temp);
  }
  if (max_label_) {
    StringCchPrintf(sz_temp, kBufferLen,  _T("%.1f C"), max_temp-kKelvinTransform);
    max_label_->SetText(sz_temp);
  }
}

void CameraImageControlUI::OnConnect() {
  img_width = camera_manage->GetImageWidth();
  align_width = (img_width + 3)&~3;
  img_height = camera_manage->GetImageHeight();
  bitmap_.Init(img_width, img_height);
  InitPalette();
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

void CameraImageControlUI::InitPalette() {
  //Init the palette
  int red_begin = 0x33, red_end = 0xf6;
  int green_begin = 0x9c, green_end = 0xaa;
  int blue_begin = 0x29, blue_end = 0x9f;
 
  int palette_count = bitmap_.GetPaletteLen();
  RGBQUAD * palette_buffer = bitmap_.GetPalettePtr();
  RGBQUAD * ui_palette_buffer = NULL;
  if (temp_color_ui_)
    ui_palette_buffer = temp_color_ui_->bitmap_.GetPalettePtr();

  for (int i = 0; i < palette_count-1; ++i) {
    RGBQUAD color = { 
      GetMidColor(red_begin,red_end,i), 
      GetMidColor(green_begin, green_end, i),
      GetMidColor(blue_begin,blue_end,i), 0 };
    palette_buffer[i] = color;
    if (ui_palette_buffer)
      ui_palette_buffer[i] = color;
  }
  RGBQUAD red_color = { 0xFF, 0, 0, 0 };
  palette_buffer[palette_count - 1] = red_color;
  if (temp_color_ui_)
    temp_color_ui_->Invalidate();
}

LPCTSTR CameraImageControlUI::GetClass() const {
  return _T("CameraImageUI");
}

void CameraImageControlUI::SetBuffer(float* val, int val_count) {

  float min_temp = (float)INT_MAX, max_temp = (float)INT_MIN;
  for (int i = 0; i < val_count; ++i) {
    min_temp = std::min(min_temp, val[i]);
    max_temp = std::max(max_temp, val[i]);
  }
  UpdateTempLabel(min_temp, max_temp);

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

/////////////TemperatureColorTableUI////////////
TemperatureColorTableUI::TemperatureColorTableUI() {
  const int kWidth = bitmap_.GetPaletteLen();
  const int kHeight = 2;
  bitmap_.Init(kWidth, kHeight);
  BYTE* buffer = bitmap_.GetBitBuffer();
  for (int i = 0; i < kWidth; ++i) {
    for (int j = 0; j < kHeight; ++j) {
      buffer[j*kWidth + i] = static_cast<BYTE>(i);
    }
  }
}
LPCTSTR TemperatureColorTableUI::GetClass() const {
  return _T("TemperatureColorControl");
}

void  TemperatureColorTableUI::PaintStatusImage(HDC hDC) {
  bitmap_.Paint(hDC,m_rcItem);
}

 
/////////////////////////CameraImageControl////////////////////
CameraImageControl::CameraImageControl()
{
  camera_info = NULL;
}

void CameraImageControl::Init(CameraManage* manage) {
  camera_info = manage;
}

CameraImageControl::~CameraImageControl()
{
  
}

DuiLib::CControlUI*  CameraImageControl::CreateControl(LPCTSTR class_name) {
  if (_tcscmp(class_name, _T("CameraImage")) == 0) {
    return new CameraImageControlUI(camera_info);
  }
  else if (_tcscmp(class_name, _T("TemperatureTable")) == 0) {
    return new TemperatureColorTableUI;
  }
  return NULL;
}