#pragma once
#include "UIlib.h"
#include "DIBitmap.h"
#include "TemperatureColorTableUI.h"
#include <memory>

namespace DuiLib {
  class CControlUI;
}

class CameraImageBuffer;
class BorderGlitterAnimation;

class CameraImageRealTimeUI : public DuiLib::CControlUI {
public:
  CameraImageRealTimeUI();
  ~CameraImageRealTimeUI();

  LPCTSTR GetClass() const;

  virtual void PaintStatusImage(HDC hDC);

  void UpdateImage(CameraImageBuffer* buffer);

private:
  void GetMinMaxFromBuffer(CameraImageBuffer* buffer);

  bool OnSizeChange(void* param);
  bool OnWindowInit(void* param);

  void RecalculateColorTablePos();
  void RecalculateLabelPos();

  void MinMaxTempChange(float min_temp, float max_temp);

  DIBitmap  bitmap_;
  float min_temp_, max_temp_;
  TemperatureColorTableUI color_table_;
  DuiLib::CLabelUI  min_temp_label_;
  DuiLib::CLabelUI  max_temp_label_;

  const static unsigned int  kWarningAnimationID = 1;
};



