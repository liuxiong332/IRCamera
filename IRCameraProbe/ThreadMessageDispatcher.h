#pragma once
#include <windows.h>


class ThreadMessageDispatcher
{
public:
  class Task {
  public:
    virtual void DoEvent() = 0;
    virtual ~Task() {}
  };
  ThreadMessageDispatcher();
  ~ThreadMessageDispatcher();
  //for the worker thread to dispatch message
  void Init(int thread_id);
  //initialize the dispatcher that the UI thread contains window
  void Init(HWND hWnd);

  void PushTask(Task* task);
  bool HandleMessage(UINT msg, WPARAM wParam,LPARAM lParam);

private:
  HWND hwnd_;
  int thread_id;
};

class ThreadMessageDispatcherManage {
public:
  ThreadMessageDispatcherManage();

  ThreadMessageDispatcher* GetMainDispatcher();
  ThreadMessageDispatcher* GetCameraDispatcher();

  void SetMainDispatcher(ThreadMessageDispatcher*);
  void SetCameraDispatcher(ThreadMessageDispatcher*);

  static ThreadMessageDispatcherManage* GetInstance();
private:
  ThreadMessageDispatcher*  main_dispatcher;
  ThreadMessageDispatcher* camera_dispatcher;
  static ThreadMessageDispatcherManage* dispatcher_manager;
};

