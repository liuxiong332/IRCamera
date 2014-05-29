// IRCameraProbe.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "UIlib.h"
#include "CameraImageControl.h"
#include "CameraManage.h"
#include "ThreadMessageDispatcher.h"
#include "CameraThread.h"

class CFrameWindowWnd : public DuiLib::CWindowWnd, //public DuiLib::INotifyUI
  public CameraManage::ConnectStatusObserver
{
public:
  CFrameWindowWnd() {  };

  LPCTSTR GetWindowClassName() const { return _T("UIMainFrame"); };
  UINT GetClassStyle() const { return UI_CLASSSTYLE_FRAME | CS_DBLCLKS; };
  void OnFinalMessage(HWND /*hWnd*/) { delete this; };

  bool ConnectBtnClick(void* param) {
    DuiLib::TNotifyUI*  notify = reinterpret_cast<DuiLib::TNotifyUI*>(param);
    if (notify->sType == _T("click")) {
      camera_manage.Connect(NULL);
      return true;
    }
    return false;
  }

  bool DisconnectBtnClick(void* param) {
    DuiLib::TNotifyUI*  notify = reinterpret_cast<DuiLib::TNotifyUI*>(param);
    if (notify->sType == _T("click")) {
      camera_manage.Disconnect();
      return true;
    }
    return false;
  }

  

  void Init() { 
    main_dispatcher.Init(GetHWND());
    ThreadMessageDispatcherManage::GetInstance()->SetMainDispatcher(&main_dispatcher);
    camera_thread_.BeginThread();
    camera_manage.Init();

    connect_btn = static_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("connect_btn")));
    disconnect_btn = static_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("disconnect_btn")));
    image_control = m_pm.FindControl(_T("camera_image"));
    ASSERT(connect_btn != NULL && disconnect_btn != NULL && image_control!=NULL);
    connect_btn->SetEnabled(false);

    connect_btn->OnNotify += MakeDelegate(this, &CFrameWindowWnd::ConnectBtnClick);
    disconnect_btn->OnNotify += MakeDelegate(this, &CFrameWindowWnd::DisconnectBtnClick);
  //  image_control->OnNotify += MakeDelegate(this, &CFrameWindowWnd::ImageCtrlTimerProc);

    camera_manage.AddConnectStatusObserver(this);
  }

  virtual void OnInitCamera() {
    connect_btn->SetEnabled(true);
  }

  virtual void  OnConnect()  {
    connect_btn->SetEnabled(false);
    disconnect_btn->SetEnabled(true);
    image_control->Invalidate();
    m_pm.SetTimer(image_control, kFlushImageTimerID, kFlushPeriodInMs);
  }

  virtual void  OnDisconnect() {
    connect_btn->SetEnabled(true);
    disconnect_btn->SetEnabled(false);
    m_pm.KillTimer(image_control, kFlushImageTimerID);
  }

  virtual void OnImageUpdate()  {}
  LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
  {
    if (uMsg == WM_CREATE) {
      m_pm.Init(m_hWnd);
      image_builder.Init(&camera_manage);
      DuiLib::CDialogBuilder builder;
      DuiLib::CControlUI* pRoot = builder.Create(_T("skin/IRCamera.xml"), (UINT)0, &image_builder, &m_pm);
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
    if (main_dispatcher.HandleMessage(uMsg, wParam, lParam)) return 0;
    LRESULT lRes = 0;
    if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
    return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
  }

public:
  CameraThread        camera_thread_;
  CameraManage        camera_manage;

  CameraImageControl  image_builder;
  DuiLib::CPaintManagerUI m_pm;
  ThreadMessageDispatcher main_dispatcher;

  DuiLib::CControlUI* image_control;
  DuiLib::CButtonUI*  disconnect_btn;
  DuiLib::CButtonUI*  connect_btn;

  const static int kFlushImageTimerID = 1;
  const static int kFlushPeriodInMs = 200;
};


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
  DuiLib::CPaintManagerUI::SetInstance(hInstance);
  DuiLib::CPaintManagerUI::SetResourcePath(DuiLib::CPaintManagerUI::GetInstancePath());

  HRESULT Hr = ::CoInitialize(NULL);
  if (FAILED(Hr)) return 0;

  CFrameWindowWnd* pFrame = new CFrameWindowWnd();
  if (pFrame == NULL) return 0;
  pFrame->Create(NULL, _T("IRCameraProbe"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
  pFrame->CenterWindow();
  pFrame->ShowWindow(true);
  DuiLib::CPaintManagerUI::MessageLoop();

  ::CoUninitialize();
  return 0;
}
