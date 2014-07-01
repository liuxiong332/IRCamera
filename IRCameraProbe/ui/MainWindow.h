#pragma once

#include "UIlib.h"
#include "core/CameraDevice.h"
#include "core/CameraDeviceObserver.h"
#include "thread/MessageQueueThread.h"
#include "thread/MessageLoop.h"

#include "ControlBuilder.h"
#include "TemperatureColorTableUI.h"
#include "CameraImageUI.h"

#include "CameraImageBuilder.h"

#include "linker/CameraImageLinkerList.h"
#include <memory>

class  CameraImageListUI;

class MainWindow : public DuiLib::CWindowWnd, //public DuiLib::INotifyUI
  public CameraImageBuilder
{
public:
  MainWindow() {  };

  LPCTSTR GetWindowClassName() const { return _T("UIMainFrame"); };
  UINT GetClassStyle() const { return UI_CLASSSTYLE_FRAME | CS_DBLCLKS; };
  void OnFinalMessage(HWND /*hWnd*/) { delete this; };

  bool OnSettingBtnClick(void* param);

  bool OnSettingBackBtnClick(void* param);
   
  void Init();

  LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
  virtual CameraImageContainerUI* CreateCameraImageContainerUI() override;

  camera::MessageQueueThread        camera_thread_;
  camera::CameraDevice        camera_manage;

  ControlBuilder      image_builder;
  DuiLib::CPaintManagerUI m_pm;

  CameraImageUI * image_control;
  DuiLib::CButtonUI*  disconnect_btn;
  DuiLib::CButtonUI*  connect_btn;
  DuiLib::CLabelUI*   min_temp_label_;
  DuiLib::CLabelUI*   max_temp_label_;
  DuiLib::CTabLayoutUI* content_layout_;
  TemperatureColorTableUI*  color_table_ui_;
   
  camera::CameraImageLinkerList linker_list_;
  std::unique_ptr<CameraImageListUI>  camera_image_list_ui_;

  const static int kFlushImageTimerID = 1;
  const static int kFlushPeriodInMs = 200;
};

