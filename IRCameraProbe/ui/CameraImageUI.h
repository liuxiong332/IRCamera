#pragma once
#include "UIlib.h"
#include "DIBitmap.h"
#include <memory>

namespace DuiLib {
  class CControlUI;
}

class CameraImageBuffer;
class BorderGlitterAnimation;

class CameraImageUI : public DuiLib::CControlUI {
public:
  CameraImageUI();
  ~CameraImageUI();

  LPCTSTR GetClass() const;
   
  virtual void PaintStatusImage(HDC hDC);

  void UpdateImage(CameraImageBuffer* buffer);
  //begin animation that present the warning information
  void BeginWarningAnimation();
  void EndWarningAnimation();
private:
  bool OnTimerProc(void* param);

  DIBitmap  bitmap_;
  std::unique_ptr<BorderGlitterAnimation> animation_;
  const static unsigned int  kWarningAnimationID = 1;
};



