#pragma once

#include "ControlBuilder.h"

#include "CameraImageBuilder.h"
#include "CameraInfoPrefBuilder.h"
#include "TabLayoutSwitcher.h"
#include "UIlib.h"
#include <memory>

class  CameraImageListUI;
class  CameraPrefLayoutUI;

namespace DuiLib {
class CControlUI;
class CTabLayoutUI;
}

class MainWindow : public DuiLib::CWindowWnd, //public DuiLib::INotifyUI
  public CameraImageBuilder,
  public CameraInfoPrefBuilder,
  public TabLayoutSwitcher
{
public:
  MainWindow();

  void ShowWindow();

  CameraImageListUI* GetCameraImageListUI();
  CameraPrefLayoutUI* GetCameraPrefLayoutUI();
private:
  LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
  LRESULT OnCreate();

  DuiLib::CControlUI* CreateControl();

  LPCTSTR GetWindowClassName() const { return _T("UIMainFrame"); };
  UINT GetClassStyle() const { return UI_CLASSSTYLE_FRAME | CS_DBLCLKS; };
//  void OnFinalMessage(HWND /*hWnd*/) { delete this; };

  //when user click the setting button
  bool OnSettingBtnClick(void* param);
  void Init();

  virtual CameraInfoUI* CreateCameraInfoUI() override;
  virtual CameraImageContainerUI* CreateCameraImageContainerUI() override;

  virtual void SwitchToContentLayout() override;
  virtual void SwitchToPrefLayout() override;

  ControlBuilder      image_builder;
  DuiLib::CPaintManagerUI m_pm;
   
  std::unique_ptr<CameraPrefLayoutUI> camera_pref_layout_ui_;
  std::unique_ptr<CameraImageListUI>  camera_image_list_ui_;
  DuiLib::CTabLayoutUI* content_layout_;
};

