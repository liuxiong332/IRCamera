#include "CameraDevice.h"
#include "IRCameraDevice.h"
#include "thread/MessageLoopManager.h"
#include "thread/MessageLoopManager.h"
#include "thread/MessageLoop.h"
#include "CameraImageBuffer.h"
#include <algorithm>
#include <assert.h>
#include <tchar.h>

namespace camera {

class  CameraManageEventHandler : public IRCameraDevice::IRCameraEventHandler {
public:
  CameraManageEventHandler(CameraDevice* manager) : manager_(manager) {}
  virtual void OnEvent(IRCameraEvent event_type) {
    manager_->CameraEventHandlerInCameraThread(event_type);
  }
private:
  CameraDevice* manager_;
};


CameraDevice::CameraDevice()
    : camera_message_loop_(NULL),
      main_message_loop_(NULL),
      camera_status_(DISCONNECTED) {
  camera_event_handler_in_camera_thread_.reset(new CameraManageEventHandler(this));
}

void CameraDevice::Init() {
  MessageLoopManager* manager = MessageLoopManager::GetInstance();
  camera_message_loop_ = manager->GetCameraMessageLoop();
  main_message_loop_ = manager->GetMainMessageLoop();

  SharedCameraDevicePtr ptr(shared_from_this());
  camera_message_loop_->PushTaskAndReply(main_message_loop_, [ptr]() {
    ptr->camera_info_ = IRCameraDevice::CreateForTest();
    ptr->camera_info_->RegisterEventHandler(ptr->camera_event_handler_in_camera_thread_.get());
  }, [ptr] {
    ptr->InitCompleteTrigger();
  });
//  camera_dispatcher_->PushTask(create_task);
//  create_task->DoEvent();
}


CameraDevice::~CameraDevice()
{
  camera_info_->RegisterEventHandler(NULL);
//   if (camera_status_ == CONNECTED)
//     Disconnect(); 
//  IRCameraDestroy(camera_info);
}

void CameraDevice::Connect(const ConnectResultObserver& observer) {
  IRCameraStatusCode* code = new IRCameraStatusCode;
  SharedCameraDevicePtr ptr(shared_from_this());
  camera_message_loop_->PushTaskAndReply(main_message_loop_, [ptr, code] {
    *code = ptr->camera_info_->Connect(_T("127.0.0.1"));
  }, [observer, code] {
    if (!observer)  return;
    observer(*code);
    delete code;
  });
//  camera_dispatcher_->PushTask(connect_task);
//  connect_task->DoEvent();
}

void CameraDevice::Disconnect(){
  SharedCameraDevicePtr ptr(shared_from_this());
//  return IRCameraDisconnect(camera_info);  camera_dispatcher_->PushTask(disconnect_task);
  camera_message_loop_->PushTask([ptr] {
    ptr->camera_info_->Disconnect();
  });
}

void  CameraDevice::AddConnectStatusObserver(CameraDeviceObserver* observer) {
  observers_.push_back(observer);
}

void CameraDevice::RemoveConnectStatusObserver(CameraDeviceObserver* observer) {
  ObserverIterator iter;
  iter = std::find(observers_.begin(), observers_.end(), observer);
  observers_.erase(iter);
}

TString CameraDevice::GetErrorString(IRCameraStatusCode code) {
  return camera_info_->GetErrorString(code);
}

int CameraDevice::GetImageWidth() const {
  return image_width_;
}
int CameraDevice::GetImageHeight() const {
  return image_height_;
}
 
int CameraDevice::GetStatus() {
  return camera_status_;
}
 
void CameraDevice::InitCompleteTrigger(){
  for (ObserverIterator iter = observers_.begin(); iter != observers_.end(); ++iter) {
    (*iter)->OnInitCamera();
  }
}
void CameraDevice::ConnectedTrigger(){
  camera_status_ = CONNECTED;
  for (ObserverIterator iter = observers_.begin(); iter != observers_.end(); ++iter) {
    (*iter)->OnConnect();
  }
}

void CameraDevice::DisconnectTrigger() {
  camera_status_ = DISCONNECTED;
  for (ObserverIterator iter = observers_.begin(); iter != observers_.end(); ++iter) {
    (*iter)->OnDisconnect();
  }
}

void CameraDevice::CameraEventHandlerInCameraThread(IRCameraEvent event_type) {
  if (event_type == IRCAMERA_CONNECTED_EVENT) {
    image_width_ = camera_info_->GetImageWidth();
    image_height_ = camera_info_->GetImageHeight();
  }
  SharedCameraDevicePtr ptr(shared_from_this());
  main_message_loop_->PushTask( [event_type, ptr]() {
    switch (event_type) {
    case IRCAMERA_CONNECTED_EVENT:
      ptr->ConnectedTrigger();
      break;
    case IRCAMERA_DISCONNECTED_EVENT:
      ptr->DisconnectTrigger();
      break;
    }
  });
}

void CameraDevice::UpdateKelvinImage() {
  SharedCameraDevicePtr ptr(shared_from_this());
  camera_message_loop_->PushTask([ptr] {
    CameraImageBuffer*  buffer;
    IRCameraStatusCode code = ptr->camera_info_->GetKelvinImage(&buffer);
    if (code == IRCAMERA_OK) {
      ptr->main_message_loop_->PushTask([ptr, buffer] {
        ptr->UpdateImageTrigger(buffer);
        delete buffer;
      });
    }
  });
}

void CameraDevice::UpdateImageTrigger(CameraImageBuffer* buffer) {
  for (ObserverIterator iter = observers_.begin(); iter != observers_.end(); ++iter) {
    (*iter)->OnImageUpdate(buffer); 
  }
}
 
}