// IRCameraProbe.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"

#include "MainWindow.h"
#include "CameraImageListUI.h"
#include "CameraPrefLayoutUI.h"
#include "CameraInfoPrefLayout.h"
#include "SampleModePrefUI.h"
#include "CameraInfoListUI.h"
#include "CameraInfoUI.h"
#include "CameraImageContainerUI.h"
#include "CanEditButtonUI.h"


bool MainWindow::OnSettingBtnClick(void* param) {
  DuiLib::TNotifyUI* notify = reinterpret_cast<DuiLib::TNotifyUI*>(param);
  if (notify->sType == _T("click")) {
    content_layout_->SelectItem(1);
    return true;
  }
  return false;
}
 
MainWindow::MainWindow() :content_layout_(NULL) {
  image_builder.SetCanEditButtonUICreator(this);
}

void MainWindow::Init() { 
  DuiLib::CControlUI* content_control = m_pm.FindControl(_T("content_layout"));
  content_layout_ = static_cast<DuiLib::CTabLayoutUI*>(content_control);

  DuiLib::CButtonUI*  setting_btn = static_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("setting_btn")));
  setting_btn->OnNotify += MakeDelegate(this, &MainWindow::OnSettingBtnClick);

  DuiLib::CContainerUI* image_list = static_cast<DuiLib::CContainerUI*>(m_pm.FindControl(_T("camera_image_list")));
  camera_image_list_ui_.reset(new CameraImageListUI);
  camera_image_list_ui_->Init(image_list);

  DuiLib::CContainerUI* camera_info_layout_ui = static_cast<DuiLib::CContainerUI*>(m_pm.FindControl(_T("pref_layout")));
  camera_pref_layout_ui_.reset(new CameraPrefLayoutUI);
  camera_pref_layout_ui_->Init(camera_info_layout_ui, this, this);
}

CameraImageListUI* MainWindow::GetCameraImageListUI() {
  return camera_image_list_ui_.get();
}

CameraPrefLayoutUI* MainWindow::GetCameraPrefLayoutUI() {
  return camera_pref_layout_ui_.get();
}

void MainWindow::SwitchToContentLayout() {
  content_layout_->SelectItem(0);
}

void MainWindow::SwitchToPrefLayout() {
  content_layout_->SelectItem(1);
}

CameraInfoUI* MainWindow::CreateCameraInfoUI() {
  DuiLib::CDialogBuilder builder;
  DuiLib::CControlUI* control = builder.Create(_T("CameraInfo.xml"), (UINT)0, &image_builder, &m_pm);
  control->SetManager(&m_pm, NULL, false);
  CameraInfoUI* container_ui = new CameraInfoUI;
  container_ui->Init(static_cast<DuiLib::CContainerUI*>(control));
  return container_ui;
}

CameraImageContainerUI* MainWindow::CreateCameraImageContainerUI() {
  DuiLib::CDialogBuilder builder;
  DuiLib::CControlUI* control = builder.Create(_T("CameraImage.xml"), (UINT)0, &image_builder, &m_pm);
  control->SetManager(&m_pm, NULL, false);
  CameraImageContainerUI* container_ui = new CameraImageContainerUI;
  container_ui->Init(static_cast<DuiLib::CContainerUI*>(control));
  return container_ui;
}

CanEditButtonUI* MainWindow::CreateCanEditButtonUI() {
  DuiLib::CDialogBuilder builder;
  DuiLib::CControlUI* control = builder.Create(_T("CanEditButton.xml"),NULL, &image_builder, &m_pm);
  control->SetManager(&m_pm, NULL, false);
  CanEditButtonUI* can_edit_ui = new CanEditButtonUI(static_cast<DuiLib::CContainerUI*>(control));
  return can_edit_ui;
}

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_CREATE) {
    return OnCreate();
  } else if (uMsg == WM_DESTROY) {
    ::PostQuitMessage(0L);
  } else if (uMsg == WM_ERASEBKGND) {
    return 1;
  }
  LRESULT lRes = 0;
  if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
  return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}
 
void MainWindow::ShowWindow() {
  Create(NULL, _T("IRCameraProbe"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
  CenterWindow();
  DuiLib::CWindowWnd::ShowWindow(true);
}

LRESULT MainWindow::OnCreate()
{
  m_pm.Init(m_hWnd);
  DuiLib::CControlUI* pRoot =  CreateControl();
  ASSERT(pRoot && "Failed to parse XML");
  m_pm.AttachDialog(pRoot);
  Init();
  return 0;
}

DuiLib::CControlUI* MainWindow::CreateControl()
{
  DuiLib::CDialogBuilder builder;
  return builder.Create(_T("IRCamera.xml"), (UINT)0, &image_builder, &m_pm);
}

