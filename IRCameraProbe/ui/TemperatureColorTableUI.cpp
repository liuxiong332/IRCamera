#include "TemperatureColorTableUI.h"


/////////////TemperatureColorTableUI////////////
TemperatureColorTableUI::TemperatureColorTableUI(bool horizontal) {
  if (horizontal)
    InitForHorizontal();
  else
    InitForVertical();
}
LPCTSTR TemperatureColorTableUI::GetClass() const {
  return _T("TemperatureColorControl");
}

void  TemperatureColorTableUI::PaintStatusImage(HDC hDC) {
  bitmap_.Paint(hDC, m_rcItem);
}

void TemperatureColorTableUI::InitForHorizontal()
{
  const int kWidth = bitmap_.GetPaletteLen();
  const int kHeight = 2;
  
  bitmap_.Init(kWidth, kHeight);
  BYTE* buffer = bitmap_.GetBitBuffer();
  for (int i = 0; i < kWidth; ++i) {
    BYTE pixel = static_cast<BYTE>(i);
    for (int j = 0; j < kHeight; ++j) {
      buffer[j*kWidth + i] = pixel;
    }
  }
}

void TemperatureColorTableUI::InitForVertical()
{
  const int kWidth = 4;
  const int kHeight = bitmap_.GetPaletteLen();
  bitmap_.Init(kWidth, kHeight);
  BYTE* buffer = bitmap_.GetBitBuffer();
  
  for (int j = 0; j < kHeight; ++j) {
    BYTE pixel = static_cast<BYTE>(kHeight - 1 - j);
    for (int i = 0; i < kWidth; ++i) {
      buffer[j*kWidth + i] = pixel;
    }
  }
}

