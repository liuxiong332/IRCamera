#pragma once
#include "MessageQueueThread.h"
#include "ThreadMessageDispatcher.h"

class CameraThreadProcessor;
class CameraThread
{
public:
  CameraThread();
  ~CameraThread();

  bool BeginThread();

  void Quit();
private:
  MessageQueueThread  thread;
  ThreadMessageDispatcher camera_dispatcher;
  MessageQueueThread::ThreadProcessor*  processor;

  friend class CameraThreadProcessor;
};

