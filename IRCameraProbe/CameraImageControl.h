#pragma once
#include "UIlib.h"
#include <windows.h>
#include "CameraManage.h"
#include <fstream>

namespace DuiLib {
  class CControlUI;
}
class CameraManage;

class DIBitmap {
public:
  DIBitmap();
  ~DIBitmap();
  void Init(int width, int height);
  BITMAPINFO* GetInfo();
  int GetPaletteLen() const;
  void SetPalette(int index, const RGBQUAD&);
  RGBQUAD*  GetPalettePtr();
  BYTE* GetBitBuffer();
  void  Paint(HDC hdc, RECT rcItem);
private:
  int img_width_, img_height_;
  BITMAPINFOHEADER*   bitmap_header_;
  BYTE* bitmap_buffer_;
  RGBQUAD*  bitmap_palette_;
  static const int PALETTE_COUNT = 256;
};

class TemperatureColorTableUI;
class CameraImageControlUI : public DuiLib::CControlUI,
  public CameraManage::ConnectStatusObserver,
  public IRCameraImageFilling
{
public:
  CameraImageControlUI(CameraManage* manager);
  ~CameraImageControlUI();

  LPCTSTR GetClass() const;

  void BindTempLabel(DuiLib::CLabelUI* min_label, DuiLib::CLabelUI* max_label);

  void BindUI(TemperatureColorTableUI* ui)  { temp_color_ui_ = ui; }

  void SetThresholdTemp(float threshold_temp);

  virtual void PaintStatusImage(HDC hDC);

  virtual void  OnInitCamera() {}
  virtual void  OnConnect();
  virtual void  OnDisconnect();
  virtual void  OnImageUpdate();
private:
  void UpdateTempLabel(float min_temp, float max_temp);

  bool ImageCtrlTimerProc(void* param);

  virtual void SetBuffer(float*, int);
  void InitBitmapHeader();
  void InitPalette();

  int align_width;
  int img_width, img_height;

  float threshold_temp_;
  float min_temp, max_temp;
  bool  is_need_update_;

  TemperatureColorTableUI*  temp_color_ui_;
  DuiLib::CLabelUI* min_label_;
  DuiLib::CLabelUI* max_label_;
  CameraManage* camera_manage;

  DIBitmap  bitmap_;

  inline int GetMidColor(int begin, int end, int i);
};

class TemperatureColorTableUI : public DuiLib::CControlUI {
public:
  TemperatureColorTableUI();
  LPCTSTR GetClass() const;
  virtual void PaintStatusImage(HDC hDC);
private:
  DIBitmap  bitmap_;
  friend class  CameraImageControlUI;
};


class CameraImageControl: public DuiLib::IDialogBuilderCallback
{
public:
  CameraImageControl();
  void Init(CameraManage* manage);
  ~CameraImageControl();

  virtual DuiLib::CControlUI* CreateControl(LPCTSTR class_name) override;
private:
  CameraManage*   camera_info;
};

