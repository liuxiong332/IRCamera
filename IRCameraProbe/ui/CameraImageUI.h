#pragma once
#include "UIlib.h"
#include <windows.h>
#include "core/CameraDevice.h"
#include "core/CameraDeviceObserver.h"
#include <fstream>
#include "DIBitmap.h"

namespace DuiLib {
  class CControlUI;
}
namespace camera {
class CameraDevice;
}

class TemperatureColorTableUI;
class CameraImageUI : public DuiLib::CControlUI {
public:
  CameraImageUI(camera::CameraDevice* manager);
  ~CameraImageUI();

  LPCTSTR GetClass() const;
   
  virtual void PaintStatusImage(HDC hDC);
private:
  bool ImageCtrlTimerProc(void* param);

  virtual void SetBuffer(float*, int);
//  void InitBitmapHeader();
//  void InitPalette();

  int align_width;
  int img_width, img_height;

  float threshold_temp_;
  float min_temp, max_temp;
  bool  is_need_update_;

  camera::CameraDevice* camera_manage;

  DIBitmap  bitmap_;
};



