#include "CameraImageRealTimeUI.h"
#include "CameraImageBuffer.h"
#include "common/TempTransform.h"
#include <algorithm>
#include <memory>
#include <windows.h>
#include <strsafe.h>
#include "common/StringUtility.h"

class TemperatureColorTableUI;
 

//////////////////////CameraImageControlUI/////////////////////////
CameraImageRealTimeUI::CameraImageRealTimeUI() :color_table_(false) {
  //  camera_manage->AddConnectStatusObserver(this );
  //  InitBitmapHeader();
  this->OnSize += DuiLib::MakeDelegate(this, &CameraImageRealTimeUI::OnSizeChange);
  OnInit += DuiLib::MakeDelegate(this, &CameraImageRealTimeUI::OnWindowInit);
}

bool CameraImageRealTimeUI::OnWindowInit(void*) {
  DuiLib::CPaintManagerUI* manager = GetManager();
  manager->InitControls(&color_table_, this);
  manager->InitControls(&min_temp_label_, this);
  manager->InitControls(&max_temp_label_, this);

  const static int kLabelTextColor = 0xFFFF9999;
  min_temp_label_.SetBkColor(kLabelTextColor);
  max_temp_label_.SetBkColor(kLabelTextColor);
  return true;
}

CameraImageRealTimeUI::~CameraImageRealTimeUI() {
  //   OnDisconnect();
  //   camera_manage->RemoveConnectStatusObserver(this);
}

LPCTSTR CameraImageRealTimeUI::GetClass() const {
  return _T("CameraImageRealTimeUI");
}

bool CameraImageRealTimeUI::OnSizeChange(void* param) {
  RecalculateColorTablePos();
  RecalculateLabelPos();
  return true;
}

void CameraImageRealTimeUI::RecalculateColorTablePos() {
  const static int kRightPadding = 5;
  const static int kVerticalPadding = 30;
  int table_width = (m_rcItem.right - m_rcItem.left) / 20;
  int right = m_rcItem.right - kRightPadding;
  int left = right - table_width;
  int top = m_rcItem.top + kVerticalPadding;
  int bottom = m_rcItem.bottom - kVerticalPadding;

//   const static int kBottomPadding = 5;
//   const static int kHorizontalPadding = 30;
//   int table_height = (m_rcItem.bottom - m_rcItem.top) / 20;
//   int bottom = m_rcItem.bottom - kBottomPadding;
//   int top = bottom - table_height;
//   int left = m_rcItem.left + kHorizontalPadding;
//   int right = m_rcItem.right - kHorizontalPadding;

  RECT rc = { left, top, right, bottom };
  color_table_.SetPos(rc);
}

void CameraImageRealTimeUI::RecalculateLabelPos() {
  const static int kRightPadding = 5;
  const static int kVerticalPadding = 30;
  int label_width = (m_rcItem.right - m_rcItem.left) / 8; 
  TEXTMETRIC* tm = &GetManager()->GetDefaultFontInfo()->tm;
  int label_height = tm->tmHeight + tm->tmExternalLeading;

  int right = m_rcItem.right - kRightPadding;
  int left = right - label_width;
  int top = kVerticalPadding + m_rcItem.top;
  int bottom = top + label_height;
  max_temp_label_.SetPos({ left, top, right, bottom });

  bottom = m_rcItem.bottom - kRightPadding;
  top = bottom - label_height;
  min_temp_label_.SetPos({ left, top, right, bottom });
}

void CameraImageRealTimeUI::GetMinMaxFromBuffer(CameraImageBuffer* buffer)
{
  float*  temp_buffer = buffer->GetBuffer();
  int width = buffer->GetWidth();
  int height = buffer->GetHeight();

  float min_temp = std::numeric_limits<float>::max();
  float max_temp = 0.0f;

  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      min_temp = std::min(min_temp, temp_buffer[j]);
      max_temp = std::max(max_temp, temp_buffer[j]);
    }
    temp_buffer += width;
  }
  min_temp = camera::TransformKelvinToCelsius(min_temp);
  max_temp = camera::TransformKelvinToCelsius(max_temp);

  MinMaxTempChange(min_temp, max_temp);
}

void CameraImageRealTimeUI::UpdateImage(CameraImageBuffer* buffer) {
  int width = buffer->GetWidth();
  int height = buffer->GetHeight();
  bitmap_.Init(width, height);
  GetMinMaxFromBuffer(buffer);

  float*  temp_buffer = buffer->GetBuffer();  
  BYTE* bitmap_buffer = bitmap_.GetBitBuffer();
  int palette_len = bitmap_.GetPaletteLen();
  float span = max_temp_ - min_temp_;
  //transform the temperature value to the bitmap byte map
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      bitmap_buffer[j] = static_cast<BYTE>((camera::TransformKelvinToCelsius(temp_buffer[j]) - min_temp_) * palette_len / span);
    }
    bitmap_buffer += width;
    temp_buffer += width;
  }
  Invalidate();
}
 
void CameraImageRealTimeUI::MinMaxTempChange(float min_temp, float max_temp) {
  min_temp_ = min_temp;
  max_temp_ = max_temp;
  min_temp_label_.SetText(StringUtility::FloatToString(min_temp).c_str());
  max_temp_label_.SetText(StringUtility::FloatToString(max_temp).c_str());
}

void CameraImageRealTimeUI::PaintStatusImage(HDC hDC) {
  if (bitmap_.IsInit()) {
    bitmap_.Paint(hDC, m_rcItem);
    color_table_.DoPaint(hDC, color_table_.GetPos());
    min_temp_label_.DoPaint(hDC, min_temp_label_.GetPos());
    max_temp_label_.DoPaint(hDC, max_temp_label_.GetPos());
  }
}

