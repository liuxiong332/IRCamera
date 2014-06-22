#pragma once
#include "UIlib.h"
#include <windows.h>
#include "CameraManage.h"
#include <fstream>
#include "DIBitmap.h"

namespace DuiLib {
  class CControlUI;
}
class CameraManage;

class TemperatureColorTableUI;
class CameraImageControlUI : public DuiLib::CControlUI,
  public CameraManage::ConnectStatusObserver,
  public IRCameraImageFilling
{
public:
  CameraImageControlUI(CameraManage* manager);
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

  CameraManage* camera_manage;

  DIBitmap  bitmap_;

  inline int GetMidColor(int begin, int end, int i);
};



