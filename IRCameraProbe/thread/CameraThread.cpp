#include "CameraThread.h"


class CameraThreadProcessor : public MessageQueueThread::ThreadProcessor {
public:
  CameraThreadProcessor(ThreadMessageDispatcher* dispatcher);
  virtual bool OnMessage(const MSG& msg);
private:
  ThreadMessageDispatcher*  dispatcher;
};

CameraThreadProcessor::CameraThreadProcessor(ThreadMessageDispatcher* dispatcher) {
  this->dispatcher = dispatcher;
}

bool CameraThreadProcessor::OnMessage(const MSG& msg) {
  return dispatcher->HandleMessage(msg.message, msg.wParam, msg.lParam);
}

CameraThread::CameraThread()
{
  processor = NULL;
}


CameraThread::~CameraThread()
{
  if (processor) {
    delete processor;
    processor = NULL;
  }
}

void CameraThread::Quit() {
  thread.Quit();
}

bool CameraThread::BeginThread() {
  processor = new CameraThreadProcessor(&camera_dispatcher);
  if (!thread.BeginThread(processor))
    return false;
  camera_dispatcher.Init(thread.GetThreadID());
  ThreadMessageDispatcherManage::GetInstance()->SetCameraDispatcher(&camera_dispatcher);
  return true;
}