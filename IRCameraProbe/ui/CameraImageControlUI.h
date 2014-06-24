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
class CameraImageControlUI : public DuiLib::CControlUI,
  public camera::CameraDeviceObserver,
  public IRCameraImageFilling
{
public:
  CameraImageControlUI(camera::CameraDevice* manager);
  ~CameraImageControlUI();

  LPCTSTR GetClass() const;

  void SetThresholdTemp(float threshold_temp);

  virtual void PaintStatusImage(HDC hDC);

  virtual void  OnInitCamera() {}
  virtual void  OnConnect();
  virtual void  OnDisconnect();
  virtual void  OnImageUpdate();
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

  inline int GetMidColor(int begin, int end, int i);
};



