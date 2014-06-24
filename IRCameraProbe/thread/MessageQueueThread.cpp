#include "MessageQueueThread.h"
#include <process.h>
#include <assert.h>
#include "MessageLoop.h"

namespace camera {
MessageQueueThread::MessageQueueThread()
{
  message_loop_.reset(new MessageLoop);
  thread_handle = NULL;
  exit_event = CreateEvent(NULL, FALSE, FALSE, NULL);
}


MessageQueueThread::~MessageQueueThread(){
}

bool MessageQueueThread::BeginThread() {
  init_event = CreateEvent(NULL, FALSE, FALSE, NULL);
  assert(init_event != NULL);
  thread_handle = _beginthreadex(NULL, 0, &MessageQueueThread::ThreadMain, this, 0, &thread_id);
  if (thread_handle != NULL) {
    ::WaitForSingleObject(init_event, INFINITE);  //wait for the child thread
  }
  CloseHandle(init_event);
  return thread_handle!=NULL;
}

unsigned __stdcall MessageQueueThread::ThreadMain(void* args) {
  return reinterpret_cast<MessageQueueThread*>(args)->ThreadFunc();
}

unsigned MessageQueueThread::ThreadFunc() {
  ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
  MSG msg;
  //invoke PeekMessage to force the thread to create the message queue
  PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE); 
  //inform the main thread to 
  SetEvent(init_event);

  //begin the message loop
  while (true) {
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
      if (msg.message == WM_QUIT)
        break;
      //the dispatch must exist, because the ActiveX need the HWND message
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    message_loop_->DispatchTasks();
  }
  
  CloseHandle((HANDLE)thread_handle);
  ::CoUninitialize();
  SetEvent(exit_event);
  _endthreadex(0);
  return 0;
}

MessageLoop*  MessageQueueThread::GetMessageLoop() {
  return message_loop_.get() ;
}

void MessageQueueThread::Quit() {
  ::PostThreadMessage(thread_id, WM_QUIT, 0, 0);
  WaitForSingleObject(exit_event, INFINITE);
  CloseHandle(exit_event);
}

int MessageQueueThread::GetThreadID() const {
  return static_cast<int>(thread_id);
}

}