#pragma once
#include <functional>
#include <vector>
#include "Mutex.h"

namespace camera {
class MessageDispatcher;
class MessageLoop {
public:
  typedef std::function<void()> CallBack;

  MessageLoop();
  void InitWithDispatcher(MessageDispatcher* dispatcher) {
    dispatcher_ = dispatcher;
  }
  //push task into the thread task loop
  void PushTask(const CallBack& task);
  void PushTask(CallBack&& task);

  void PushTaskAndReply(MessageLoop* host_loop, CallBack&& task, CallBack&& reply);

  //run the message loop
  void RunMessageLoop();
  void DispatchTasks();

  ~MessageLoop();
private:
  void  ScheduleWork();
  void  InitMessageWnd();

  static void TaskReplyWrapper(MessageLoop* host_loop, CallBack& task, CallBack& reply);

  Mutex insert_mutex_;
  ATOM  atom_;
  HWND  message_hwnd_;
  MessageDispatcher*  dispatcher_;

  std::vector<CallBack> insert_task_list_;
  std::vector<CallBack> consume_task_list_;
  
};

}