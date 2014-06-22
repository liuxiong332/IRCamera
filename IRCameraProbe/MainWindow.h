#pragma once

#include "UIlib.h"
#include "CameraManage.h"
#include "ThreadMessageDispatcher.h"
#include "CameraThread.h"

#include "ControlBuilder.h"
#include "TemperatureColorTableUI.h"
#include "CameraImageControlUI.h"

class MainWindow : public DuiLib::CWindowWnd, //public DuiLib::INotifyUI
  public CameraManage::ConnectStatusObserver
{
public:
  MainWindow() {  };

  LPCTSTR GetWindowClassName() const { return _T("UIMainFrame"); };
  UINT GetClassStyle() const { return UI_CLASSSTYLE_FRAME | CS_DBLCLKS; };
  void OnFinalMessage(HWND /*hWnd*/) { delete this; };

  bool ConnectBtnClick(void* param);

  bool OnSettingBtnClick(void* param);

  bool OnSettingBackBtnClick(void* param);

  bool DisconnectBtnClick(void* param);
   
  void Init();

  virtual void OnInitCamera();

  virtual void  OnConnect();

  virtual void  OnDisconnect();

  virtual void OnImageUpdate()  {}
  LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
  CameraThread        camera_thread_;
  CameraManage        camera_manage;

  ControlBuilder      image_builder;
  DuiLib::CPaintManagerUI m_pm;
  ThreadMessageDispatcher main_dispatcher;

  CameraImageControlUI * image_control;
  DuiLib::CButtonUI*  disconnect_btn;
  DuiLib::CButtonUI*  connect_btn;
  DuiLib::CLabelUI*   min_temp_label_;
  DuiLib::CLabelUI*   max_temp_label_;
  DuiLib::CTabLayoutUI* content_layout_;
  TemperatureColorTableUI*  color_table_ui_;

  const static int kFlushImageTimerID = 1;
  const static int kFlushPeriodInMs = 200;
};

