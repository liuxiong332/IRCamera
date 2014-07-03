#include "ui/MainWindow.h"
#include "ui/CameraPrefLayoutUI.h"
#include "ui/CameraInfoPrefLayout.h"
#include "ui/CameraImageListUI.h"
#include "ui/CameraInfoListUI.h"
#include "ui/CameraImageContainerUI.h"
#include "ui/CameraInfoUI.h"

#include "thread/MessageLoopManager.h"

#include "linker/MainLinker.h"
#include "linker/CameraPrefLinker.h"
#include "linker/CameraImageLinkerList.h"
#include "thread/MessageDispatcher.h"

void  InitPath() {
  DuiLib::CStdString  res_path = DuiLib::CPaintManagerUI::GetInstancePath();
  if (res_path.Right(1) != _T("\\"))
    res_path += _T('\\');
  res_path += _T("skin\\");
  DuiLib::CPaintManagerUI::SetResourcePath(res_path);
}

class MainMessageDispatcher : public camera::MessageDispatcher {
public:
  virtual void Dispatch(const MSG& msg) {
    if (!DuiLib::CPaintManagerUI::TranslateMessage((LPMSG)&msg)) {
      ::TranslateMessage(&msg);
      ::DispatchMessage(&msg);
    }
  }
};

void MainRunner();

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
  DuiLib::CPaintManagerUI::SetInstance(hInstance);
  InitPath();

  HRESULT Hr = ::CoInitialize(NULL);
  if (FAILED(Hr)) return 0;

  camera::MessageLoopManager* manager = camera::MessageLoopManager::GetInstance();
  MainMessageDispatcher main_dispatcher;
  manager->Init(&main_dispatcher);

  MainRunner();

  manager->QuitCameraThread();
  ::CoUninitialize();
  return 0;
}

//run the main program
void MainRunner()
{
  camera::MessageLoop* loop = camera::MessageLoopManager::GetInstance()->GetMainMessageLoop();
  camera::MainLinker  main_linker;
  main_linker.Init();

  loop->RunMessageLoop();
}
