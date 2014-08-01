#pragma once
#include "UIlib.h"
#include "DIBitmap.h"

class TemperatureColorTableUI : public DuiLib::CControlUI {
public:
  TemperatureColorTableUI(bool hoizontal = true);

  LPCTSTR GetClass() const;
  virtual void PaintStatusImage(HDC hDC);
private:
  void InitForHorizontal();
  void InitForVertical();

  DIBitmap  bitmap_;
};