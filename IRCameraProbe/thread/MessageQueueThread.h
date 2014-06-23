#pragma once
#include <windows.h>
#include <memory>

namespace camera {

class MessageLoop;
class MessageQueueThread
{
public:
  MessageQueueThread();
  ~MessageQueueThread();

  //must invoke in the other thread
  void  Quit();
  bool  BeginThread();
  int   GetThreadID() const;

  MessageLoop*  GetMessageLoop();
private:
  unsigned ThreadFunc();

  static  unsigned __stdcall  ThreadMain(void*);
  //the event used for the host to wait the child thread initialize completely
  HANDLE    init_event;   
  HANDLE    exit_event;

  unsigned  thread_id;
  uintptr_t  thread_handle;

  std::unique_ptr<MessageLoop>  message_loop_;
};

}