#include "MessageLoopManager.h"
#include <assert.h>
 
namespace camera {

MessageLoopManager::MessageLoopManager() {
}

void MessageLoopManager::Init(MessageDispatcher* main_dispatcher) {
  main_message_loop_.InitWithDispatcher(main_dispatcher);
  camera_thread_.BeginThread();
}

MessageLoop*  MessageLoopManager::GetMainMessageLoop() {
  return &main_message_loop_;
}

MessageLoop* MessageLoopManager::GetCameraMessageLoop() {
  return camera_thread_.GetMessageLoop();
}
 
void  MessageLoopManager::QuitCameraThread() {
  camera_thread_.Quit();
}

MessageLoopManager*  MessageLoopManager::message_loop_manager_ = NULL;
MessageLoopManager*  MessageLoopManager::GetInstance() {
  static MessageLoopManager kManager;
  if (!message_loop_manager_) {
    //don't have the multithread security
    message_loop_manager_ = &kManager;
  }
  return message_loop_manager_;
}

}