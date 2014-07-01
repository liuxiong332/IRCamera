// IRCameraProbe.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"

#include "MainWindow.h"
#include "thread/MessageLoopManager.h"
#include "CameraImageContainerUI.h"
#include "CameraImageListUI.h"
#include "linker/CameraImageContainerDeviceLinker.h"

bool MainWindow::OnSettingBtnClick(void* param) {
  DuiLib::TNotifyUI* notify = reinterpret_cast<DuiLib::TNotifyUI*>(param);
  if (notify->sType == _T("click")) {
    content_layout_->SelectItem(1);
    return true;
  }
  return false;
}

bool MainWindow::OnSettingBackBtnClick(void* param) {
  DuiLib::TNotifyUI* notify = reinterpret_cast<DuiLib::TNotifyUI*>(param);
  if (notify->sType == _T("click")) {
    content_layout_->SelectItem(0);
    return true;
  }
  return false;
}

void MainWindow::Init() { 
  camera_thread_.BeginThread();
  camera::MessageLoopManager::GetInstance()->SetCameraMessageLoop(camera_thread_.GetMessageLoop());
  camera_manage.Init();


  content_layout_ = static_cast<DuiLib::CTabLayoutUI*>(m_pm.FindControl(_T("content_layout")));
  DuiLib::CButtonUI*  setting_btn = static_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("setting_btn")));
  setting_btn->OnNotify += MakeDelegate(this, &MainWindow::OnSettingBtnClick);

  DuiLib::CButtonUI*  setting_back_btn = static_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("from_setting_back_btn")));
  setting_back_btn->OnNotify += MakeDelegate(this, &MainWindow::OnSettingBackBtnClick);

  DuiLib::CContainerUI* image_list = static_cast<DuiLib::CContainerUI*>(m_pm.FindControl(_T("camera_image_list")));
  camera_image_list_ui_.reset(new CameraImageListUI);
  camera_image_list_ui_->Init(image_list);

  linker_list_.Init(this, camera_image_list_ui_.get() );

//     image_control->BindUI(color_table_ui_);
//     image_control->BindTempLabel(min_temp_label_, max_temp_label_);


//     DuiLib::CControlUI* edit_ctrl = m_pm.FindControl(_T("threshold_edit"));
//     edit_ctrl->OnNotify += MakeDelegate(this, &CFrameWindowWnd::ThresholdEditTextChange);

//  image_control->OnNotify += MakeDelegate(this, &CFrameWindowWnd::ImageCtrlTimerProc);

}

CameraImageContainerUI* MainWindow::CreateCameraImageContainerUI() {
  DuiLib::CDialogBuilder builder;
  DuiLib::CControlUI* control = builder.Create(_T("CameraImage.xml"), (UINT)0, &image_builder, &m_pm);
  CameraImageContainerUI* container_ui = new CameraImageContainerUI;
  container_ui->Init(static_cast<DuiLib::CContainerUI*>(control));
  return container_ui;
}

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_CREATE) {
    m_pm.Init(m_hWnd);
 //   image_builder.Init(&camera_manage);
    DuiLib::CDialogBuilder builder;
    DuiLib::CControlUI* pRoot = builder.Create(_T("IRCamera.xml"), (UINT)0, &image_builder, &m_pm);
    ASSERT(pRoot && "Failed to parse XML");
    m_pm.AttachDialog(pRoot);
//     m_pm.AddNotifier(this);
    Init();
    return 0;
  }
  else if (uMsg == WM_DESTROY) {
    camera_thread_.Quit();
    ::PostQuitMessage(0L);
  }
  else if (uMsg == WM_ERASEBKGND) {
    return 1;
  }
  LRESULT lRes = 0;
  if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
  return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}
 
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
  camera::MessageLoop  message_loop;
  camera::MessageLoopManager::GetInstance()->SetMainMessageLoop(&message_loop);

  DuiLib::CPaintManagerUI::SetInstance(hInstance);
  DuiLib::CStdString  res_path = DuiLib::CPaintManagerUI::GetInstancePath();
  if (res_path.Right(1) != _T("\\"))
    res_path += _T('\\');
  res_path += _T("skin\\");
  DuiLib::CPaintManagerUI::SetResourcePath(res_path);

  HRESULT Hr = ::CoInitialize(NULL);
  if (FAILED(Hr)) return 0;

  MainWindow* pFrame = new MainWindow();
  if (pFrame == NULL) return 0;
  pFrame->Create(NULL, _T("IRCameraProbe"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
  pFrame->CenterWindow();
  pFrame->ShowWindow(true);

  MSG msg = { 0 };
  while (true) {
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
      if (msg.message == WM_QUIT) 
        break;
      if (!DuiLib::CPaintManagerUI::TranslateMessage(&msg)) {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
      }
    }
    message_loop.DispatchTasks();
  }

  ::CoUninitialize();
  return 0;
}
