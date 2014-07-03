#include "MessageLoop.h"
#include "MessageDispatcher.h"
#include <tchar.h>
#include <utility>
#include <algorithm>

namespace camera {

namespace {

//Get module HINSTANCE from the module address
HMODULE GetModuleFromAddress(void* address) {
  HMODULE instance = NULL;
  if (!::GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
    GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
    static_cast<char*>(address),
    &instance)) {
  }
  return instance;
}

const int kHasWorkMsg = WM_USER + 1;
// message hwnd's window process
LRESULT CALLBACK WndProcThunk(
  HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
  switch (message) {
  case kHasWorkMsg: 
    reinterpret_cast<MessageLoop*>(wparam)->DispatchTasks();
    break;
  }
  return DefWindowProc(hwnd, message, wparam, lparam);
}
}

MessageLoop::MessageLoop() :dispatcher_(NULL) {
  InitMessageWnd();
}

void MessageLoop::PushTask(const CallBack& task) {
  ScopedMutex scoped_mutex(&insert_mutex_);
  insert_task_list_.push_back(task);
  ScheduleWork();
}
void MessageLoop::PushTask(CallBack&& task) {
  ScopedMutex scoped_mutex(&insert_mutex_);
  insert_task_list_.push_back(std::move(task));
  ScheduleWork();
}

void MessageLoop::TaskReplyWrapper(MessageLoop* host_loop, CallBack& task, CallBack& reply) {
  task();
  host_loop->PushTask(std::move(reply));
}

void MessageLoop::PushTaskAndReply(MessageLoop* host_loop, CallBack&& task, CallBack&& reply) {
  PushTask(std::bind(MessageLoop::TaskReplyWrapper, host_loop, std::move(task), std::move(reply) ));
}

void MessageLoop::ScheduleWork() {
  //send a fake message to notify that has work to do
  PostMessage(message_hwnd_, kHasWorkMsg, reinterpret_cast<WPARAM>(this), 0);
}

void MessageLoop::DispatchTasks() {
  {
    ScopedMutex scoped_mutex(&insert_mutex_);
    //move all the task that inserted to the consume task list
    insert_task_list_.swap(consume_task_list_);
  }
  typedef std::vector<CallBack>::iterator Iter;
  //for each callback in the consume task list, invoke the callback
  std::for_each(consume_task_list_.begin(),consume_task_list_.end(),
    [](const CallBack& callback) {
    callback();
  });
  consume_task_list_.clear();
}

void  MessageLoop::RunMessageLoop() {
  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0)) {
    if (dispatcher_)  dispatcher_->Dispatch(msg);
    else {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }
}

void  MessageLoop::InitMessageWnd() {
  TCHAR class_name[100];
  wsprintf(class_name, _T("FLIRMessageLoop%p"), this);

  HINSTANCE instance = GetModuleFromAddress(&WndProcThunk);
  WNDCLASSEX wc = { 0 };
  wc.cbSize = sizeof(wc);
  wc.lpfnWndProc = WndProcThunk;
  wc.hInstance = instance;
  wc.lpszClassName = class_name;
  atom_ = RegisterClassEx(&wc);

  message_hwnd_ = CreateWindow(MAKEINTATOM(atom_), 0, 0, 0, 0, 0, 0,
    HWND_MESSAGE, 0, instance, 0);
}

MessageLoop::~MessageLoop() {
  DestroyWindow(message_hwnd_);
  UnregisterClass(MAKEINTATOM(atom_), GetModuleFromAddress(&WndProcThunk));
}


}