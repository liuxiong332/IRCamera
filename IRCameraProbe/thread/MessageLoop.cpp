#include "stdafx.h"
#include "MessageLoop.h"
#include <utility>
#include <algorithm>

namespace camera {

void MessageLoop::PushTask(const CallBack& task) {
  ScopedMutex scoped_mutex(&insert_mutex_);
  insert_task_list_.push_back(task);
}
void MessageLoop::PushTask(CallBack&& task) {
  ScopedMutex scoped_mutex(&insert_mutex_);
  insert_task_list_.push_back(std::move(task));
}

void MessageLoop::TaskReplyWrapper(MessageLoop* host_loop, CallBack& task, CallBack& reply) {
  task();
  host_loop->PushTask(std::move(reply));
}

void MessageLoop::PushTaskAndReply(MessageLoop* host_loop, CallBack&& task, CallBack&& reply) {
  PushTask(std::bind(MessageLoop::TaskReplyWrapper, host_loop, std::move(task), std::move(reply) ));
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
}