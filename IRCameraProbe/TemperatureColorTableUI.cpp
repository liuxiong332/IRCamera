#include "TemperatureColorTableUI.h"


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
  bitmap_.Paint(hDC, m_rcItem);
}

