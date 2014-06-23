#pragma once
#include <windows.h>

namespace camera {
class MessageLoop;
class MessageLoopManager {
public:
  MessageLoopManager();

  MessageLoop* GetMainMessageLoop();
  MessageLoop* GetCameraMessageLoop();

  void SetMainMessageLoop(MessageLoop*);
  void SetCameraMessageLoop(MessageLoop*);

  static MessageLoopManager* GetInstance();
private:
  MessageLoop*  main_message_loop_;
  MessageLoop* camera_message_loop_;
  static MessageLoopManager* message_loop_manager_;
};

}