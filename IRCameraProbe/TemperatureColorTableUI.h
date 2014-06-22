#pragma once
#include "UIlib.h"
#include "DIBitmap.h"

class TemperatureColorTableUI : public DuiLib::CControlUI {
public:
  TemperatureColorTableUI();
  LPCTSTR GetClass() const;
  virtual void PaintStatusImage(HDC hDC);
private:
  DIBitmap  bitmap_;
};