#pragma once
#include "MessageLoop.h"
#include "MessageQueueThread.h"
#include <windows.h>

namespace camera {
class MessageDispatcher;
class MessageLoopManager {
public:
  MessageLoopManager();

  void Init(MessageDispatcher* main_dispatcher);

  MessageLoop* GetMainMessageLoop();
  MessageLoop* GetCameraMessageLoop();

  void  QuitCameraThread();

  static MessageLoopManager* GetInstance();
private:
  MessageQueueThread  camera_thread_;
  MessageLoop  main_message_loop_;
  static MessageLoopManager* message_loop_manager_;
};

}