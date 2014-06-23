#include "CameraManage.h"
#include "IRCameraDevice.h"
#include "thread/MessageLoopManager.h"
#include "thread/MessageLoopManager.h"
#include "thread/MessageLoop.h"
#include <algorithm>
#include <assert.h>
#include <tchar.h>

namespace camera {

class  CameraManageEventHandler : public IRCameraDevice::IRCameraEventHandler {
public:
  CameraManageEventHandler(CameraManage* manager) : manager_(manager) {}
  virtual void OnEvent(IRCameraEvent event_type) {
    manager_->CameraEventHandlerInCameraThread(event_type);
  }
private:
  CameraManage* manager_;
};


CameraManage::CameraManage()
    : camera_message_loop_(NULL),
      main_message_loop_(NULL),
      camera_status_(DISCONNECTED) {
  camera_event_handler_in_camera_thread_.reset(new CameraManageEventHandler(this));
}

void CameraManage::Init() {
  MessageLoopManager* manager = MessageLoopManager::GetInstance();
  camera_message_loop_ = manager->GetCameraMessageLoop();
  main_message_loop_ = manager->GetMainMessageLoop();

  camera_message_loop_->PushTaskAndReply(main_message_loop_, [this]() {
    camera_info_ = IRCameraDevice::CreateForTest();
    camera_info_->RegisterEventHandler(camera_event_handler_in_camera_thread_.get());
  }, [this] {
    InitCompleteTrigger();
  });
//  camera_dispatcher_->PushTask(create_task);
//  create_task->DoEvent();
}


CameraManage::~CameraManage()
{
  if (camera_status_ == CONNECTED)
    Disconnect();
//  IRCameraDestroy(camera_info);
}

void CameraManage::Connect(ConnectResultObserver* observer) {
  IRCameraStatusCode code;
  camera_message_loop_->PushTaskAndReply(main_message_loop_, [this, &code] {
    code = camera_info_->Connect(_T("127.0.0.1"));
  }, [this, observer, &code] {
    if (code == IRCAMERA_OK)
      observer->OnConnectSuccess();
    else
      observer->OnConnectFalied(code);
  });
//  camera_dispatcher_->PushTask(connect_task);
//  connect_task->DoEvent();
}

void CameraManage::Disconnect(){
//  return IRCameraDisconnect(camera_info);  camera_dispatcher_->PushTask(disconnect_task);
  camera_message_loop_->PushTask([this] {
    camera_info_->Disconnect();
  });
}

void  CameraManage::AddConnectStatusObserver(ConnectStatusObserver* observer) {
  observers_.push_back(observer);
}

void CameraManage::RemoveConnectStatusObserver(ConnectStatusObserver* observer) {
  ObserverIterator iter;
  iter = std::find(observers_.begin(), observers_.end(), observer);
  observers_.erase(iter);
}

TString CameraManage::GetErrorString(IRCameraStatusCode code) {
  return camera_info_->GetErrorString(code);
}

int CameraManage::GetImageWidth() const {
  return image_width_;
}
int CameraManage::GetImageHeight() const {
  return image_height_;
}
 
int CameraManage::GetStatus() {
  return camera_status_;
}
 
void CameraManage::InitCompleteTrigger(){
  for (ObserverIterator iter = observers_.begin(); iter != observers_.end(); ++iter) {
    (*iter)->OnInitCamera();
  }
}
void CameraManage::ConnectedTrigger(){
  camera_status_ = CONNECTED;
  for (ObserverIterator iter = observers_.begin(); iter != observers_.end(); ++iter) {
    (*iter)->OnConnect();
  }
}

void CameraManage::DisconnectTrigger() {
  camera_status_ = DISCONNECTED;
  for (ObserverIterator iter = observers_.begin(); iter != observers_.end(); ++iter) {
    (*iter)->OnDisconnect();
  }
}

void CameraManage::CameraEventHandlerInCameraThread(IRCameraEvent event_type) {
  if (event_type == IRCAMERA_CONNECTED_EVENT) {
    image_width_ = camera_info_->GetImageWidth();
    image_height_ = camera_info_->GetImageHeight();
  }
  main_message_loop_->PushTask( [event_type, this]() {
    switch (event_type) {
    case IRCAMERA_CONNECTED_EVENT:
      ConnectedTrigger();
      break;
    case IRCAMERA_DISCONNECTED_EVENT:
      DisconnectTrigger();
      break;
    }
  });
}

void CameraManage::UpdateKelvinImage(IRCameraImageFilling* img_filling) {
  camera_message_loop_->PushTask([this, img_filling] {
    IRCameraStatusCode code = camera_info_->GetKelvinImage(img_filling);
    if (code == IRCAMERA_OK) {
      main_message_loop_->PushTask([this] {
        UpdateImageTrigger();
      });
    }
  });
}

void CameraManage::UpdateImageTrigger() {
  for (ObserverIterator iter = observers_.begin(); iter != observers_.end(); ++iter) {
    (*iter)->OnImageUpdate();;
  }
}
 
}