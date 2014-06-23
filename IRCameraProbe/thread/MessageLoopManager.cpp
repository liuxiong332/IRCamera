#include "MessageLoopManager.h"
#include <assert.h>
 
namespace camera {

MessageLoopManager::MessageLoopManager() {
}

MessageLoop*  MessageLoopManager::GetMainMessageLoop() {
  return main_message_loop_;
}

MessageLoop* MessageLoopManager::GetCameraMessageLoop() {
  return camera_message_loop_;
}

void MessageLoopManager::SetCameraMessageLoop(MessageLoop* dispatcher) {
  camera_message_loop_ = dispatcher;
}

void MessageLoopManager::SetMainMessageLoop(MessageLoop* dispatcher) {
  main_message_loop_ = dispatcher;
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