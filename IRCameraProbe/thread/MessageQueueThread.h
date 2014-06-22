#pragma once
#include <windows.h>

class MessageQueueThread
{
public:
  class  ThreadProcessor {
  public:
    virtual bool OnMessage(const MSG& msg) = 0;
    virtual ~ThreadProcessor()  {}
  };

  MessageQueueThread();
  ~MessageQueueThread();

  //must invoke in the other thread
  void  Quit();
  bool  BeginThread(ThreadProcessor* processor);
  int   GetThreadID() const;
private:
  unsigned ThreadFunc();

  static  unsigned __stdcall  ThreadMain(void*);
  //the event used for the host to wait the child thread initialize completely
  HANDLE    init_event;   
  HANDLE    exit_event;
  unsigned  thread_id;
  uintptr_t  thread_handle;

  ThreadProcessor*  thread_process;
};

