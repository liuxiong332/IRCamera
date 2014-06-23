#pragma once
#include <functional>
#include <vector>
#include "Mutex.h"

namespace camera {
class MessageLoop {
public:
  typedef std::function<void()> CallBack;
  //push task into the thread task loop
  void PushTask(const CallBack& task);
  void PushTask(CallBack&& task);

  void PushTaskAndReply(MessageLoop* host_loop, CallBack&& task, CallBack&& reply);
  void DispatchTasks();
private:
  static void TaskReplyWrapper(MessageLoop* host_loop, CallBack& task, CallBack& reply);

  Mutex     insert_mutex_;
  std::vector<CallBack> insert_task_list_;
  std::vector<CallBack> consume_task_list_;
};

}