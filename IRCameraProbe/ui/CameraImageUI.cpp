#include "CameraImageUI.h"
#include "CameraImageBuffer.h"
#include <algorithm>
#include <memory>
#include <windows.h>
#include <strsafe.h>

namespace {
  const static float kKelvinTransform = 273.15f;
}

class TemperatureColorTableUI;

class  BorderGlitterAnimation {
public:
  BorderGlitterAnimation(DuiLib::CControlUI* ui)
      : animation_index_(0),
        ui_(ui) {
  }

  void OnStep() {
    const static DWORD kAnimationColors[] = { 0xFFFFFFFF, 0xFFFF0000, 0xFF00FFFF };
    const static int  kAnimationColorsLen = sizeof(kAnimationColors) / sizeof(kAnimationColors[0]);
    ui_->SetBorderColor(kAnimationColors[animation_index_]);
    animation_index_ = (animation_index_ + 1) % kAnimationColorsLen;
  }
private:
  DuiLib::CControlUI* ui_;
  int animation_index_;
};

//////////////////////CameraImageControlUI/////////////////////////
CameraImageUI::CameraImageUI() {
  //  camera_manage->AddConnectStatusObserver(this );
//  InitBitmapHeader();
  this->OnNotify += MakeDelegate(this, &CameraImageUI::OnTimerProc);
  
}
 
CameraImageUI::~CameraImageUI() {
//   OnDisconnect();
//   camera_manage->RemoveConnectStatusObserver(this);
}
 
LPCTSTR CameraImageUI::GetClass() const {
  return _T("CameraImageUI");
}

void CameraImageUI::UpdateImage(CameraImageBuffer* buffer) {
  float*  temp_buffer = buffer->GetBuffer();
  int width = buffer->GetWidth();
  int height = buffer->GetHeight();
  bitmap_.Init(width, height);

  BYTE* bitmap_buffer = bitmap_.GetBitBuffer();
  int palette_len = bitmap_.GetPaletteLen();

  const static float kKelvinTemp = 273.15f;
  float min_temp = 0.0f + kKelvinTemp;
  int span = 100;
  //transform the temperature value to the bitmap byte map
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      bitmap_buffer[j] = static_cast<BYTE>((temp_buffer[j] - min_temp) * palette_len / span);
    }
    bitmap_buffer += width;
    temp_buffer += width;
  }
  Invalidate();
}

bool CameraImageUI::OnTimerProc(void* param) {
  DuiLib::TNotifyUI* notify = static_cast<DuiLib::TNotifyUI*>(param);
  if (notify->sType != _T("timer")) {
    return false;
  }
  animation_->OnStep();
  Invalidate();
  return true;
}
void CameraImageUI::BeginWarningAnimation() {
  const static unsigned int kAnimationElapse = 500;
  GetManager()->SetTimer(this, kWarningAnimationID, kAnimationElapse);
  animation_.reset(new BorderGlitterAnimation(this));
}

void CameraImageUI::EndWarningAnimation() {
  GetManager()->KillTimer(this);
}
void CameraImageUI::PaintStatusImage(HDC hDC) {
  bitmap_.Paint(hDC, m_rcItem);
}

