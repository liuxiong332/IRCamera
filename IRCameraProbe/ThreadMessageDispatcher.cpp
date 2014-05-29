#include "ThreadMessageDispatcher.h"
#include <assert.h>

const static int kCustomMsgID = WM_USER + 1;
ThreadMessageDispatcher::ThreadMessageDispatcher():thread_id(0),hwnd_(NULL)  {}

void ThreadMessageDispatcher::Init(int thread_id)
{
  this->thread_id = thread_id;
}

void ThreadMessageDispatcher::Init(HWND hwnd) {
  this->hwnd_ = hwnd;
}

ThreadMessageDispatcher::~ThreadMessageDispatcher()
{
}

void ThreadMessageDispatcher::PushTask(Task* task) {
  if (thread_id)
    ::PostThreadMessage(thread_id, kCustomMsgID, (WPARAM)task, 0);
  else if (hwnd_) {
    ::PostMessage(hwnd_, kCustomMsgID, (WPARAM)task, 0);
  }
}

bool ThreadMessageDispatcher::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) {
  if (msg != kCustomMsgID) {
    return false;
  }
  Task* task = (Task*)wParam;
  task->DoEvent();
  return true;
}


ThreadMessageDispatcherManage::ThreadMessageDispatcherManage() {
  main_dispatcher = NULL;
  camera_dispatcher = NULL;
}

ThreadMessageDispatcher*  ThreadMessageDispatcherManage::GetMainDispatcher() {
  return main_dispatcher;
}

ThreadMessageDispatcher* ThreadMessageDispatcherManage::GetCameraDispatcher() {
  return camera_dispatcher;
}

void ThreadMessageDispatcherManage::SetCameraDispatcher(ThreadMessageDispatcher* dispatcher) {
  camera_dispatcher = dispatcher;
}

void ThreadMessageDispatcherManage::SetMainDispatcher(ThreadMessageDispatcher* dispatcher) {
  main_dispatcher = dispatcher;
}

ThreadMessageDispatcherManage*  ThreadMessageDispatcherManage::dispatcher_manager = NULL;
ThreadMessageDispatcherManage*  ThreadMessageDispatcherManage::GetInstance() {
  if (!dispatcher_manager) {
    //don't have the multithread security
    dispatcher_manager = new ThreadMessageDispatcherManage; 
  }
  return dispatcher_manager;
}