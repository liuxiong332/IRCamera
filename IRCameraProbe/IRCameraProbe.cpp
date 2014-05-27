// IRCameraProbe.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "UIlib.h"
#include "SkOSWindow_Win.h"

class CFrameWindowWnd : public DuiLib::CWindowWnd, public DuiLib::INotifyUI
{
public:
  CFrameWindowWnd() { };
  LPCTSTR GetWindowClassName() const { return _T("UIMainFrame"); };
  UINT GetClassStyle() const { return UI_CLASSSTYLE_FRAME | CS_DBLCLKS; };
  void OnFinalMessage(HWND /*hWnd*/) { delete this; };

  void Init() { }


  void Notify(DuiLib::TNotifyUI& msg)
  {
 
  }

  LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
  {
    if (uMsg == WM_CREATE) {
      m_pm.Init(m_hWnd);
      DuiLib::CDialogBuilder builder;
      DuiLib::CControlUI* pRoot = builder.Create(_T("test1.xml"), (UINT)0, NULL, &m_pm);
      ASSERT(pRoot && "Failed to parse XML");
      m_pm.AttachDialog(pRoot);
      m_pm.AddNotifier(this);
      Init();
      return 0;
    }
    else if (uMsg == WM_DESTROY) {
      ::PostQuitMessage(0L);
    }
    else if (uMsg == WM_ERASEBKGND) {
      return 1;
    }
    LRESULT lRes = 0;
    if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
    return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
  }

public:
  
  DuiLib::CPaintManagerUI m_pm;
};


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
  DuiLib::CPaintManagerUI::SetInstance(hInstance);
  DuiLib::CPaintManagerUI::SetResourcePath(DuiLib::CPaintManagerUI::GetInstancePath());

  HRESULT Hr = ::CoInitialize(NULL);
  if (FAILED(Hr)) return 0;

  CFrameWindowWnd* pFrame = new CFrameWindowWnd();
  if (pFrame == NULL) return 0;
  HWND hWnd = pFrame->Create(NULL, _T("这是一个最简单的测试用exe，修改test1.xml就可以看到效果"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
  SkOSWindow*   sk_window = new SkOSWindow(hWnd);
  sk_window;
  pFrame->CenterWindow();
  pFrame->ShowWindow(true);
  DuiLib::CPaintManagerUI::MessageLoop();

  ::CoUninitialize();
  return 0;
}
