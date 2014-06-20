#include "CameraManage.h"
#include "IRCameraDevice.h"
#include <algorithm>
#include "ThreadMessageDispatcher.h"
#include <assert.h>
#include <tchar.h>

class  CameraEventHandlerTask : public ThreadMessageDispatcher::Task {
public:
  CameraEventHandlerTask(CameraManage* manager) :manager_(manager)  {}
  void SetEventType(int event_type) { event_type_ = event_type;  }
  //when the camera event trigger
  virtual void DoEvent() {
    switch (event_type_) {
    case IRCAMERA_CONNECTED_EVENT:
      manager_->ConnectedTrigger();
      break;
    case IRCAMERA_DISCONNECTED_EVENT:
      manager_->DisconnectTrigger();
      break;
    }
  }
private:
  int event_type_;
  CameraManage* manager_;
};

class  CameraManageEventHandler : public IRCameraDevice::IRCameraEventHandler {
public:
  CameraManageEventHandler(CameraManage* manager) : manager_(manager) {}
  virtual void OnEvent(IRCameraEvent event_type) {
    if (event_type == IRCAMERA_CONNECTED_EVENT) {
      manager_->image_width_ = manager_->camera_info->GetImageWidth();
      manager_->image_height_ = manager_->camera_info->GetImageHeight();
    }
    CameraEventHandlerTask* handler_task = static_cast<CameraEventHandlerTask*>(manager_->event_handler_task_);
    handler_task->SetEventType(event_type);
    manager_->main_dispatcher_->PushTask(manager_->event_handler_task_);
  }
private:
  CameraManage* manager_;
};

////////////////////////CameraCreateTask/////////////////
//init the IRCameraInfo object
class CameraCreateTask : public ThreadMessageDispatcher::Task {
public:
  CameraCreateTask(CameraManage* manage) :manager_(manage)  {}
  virtual void DoEvent() {
    manager_->camera_info = IRCameraDevice::CreateForTest();
    manager_->camera_info->RegisterEventHandler(manager_->event_handler_.get());
    //notify the main thread that the camera initialize complete
    manager_->main_dispatcher_->PushTask(manager_->create_reply_task);
  }
private:
  CameraManage* manager_;
};
////////////////////////CameraCreateReplyTask///////////
//when the IRCameraInfo create successfully, the reply will send to the main thread
class CameraCreateReplyTask : public ThreadMessageDispatcher::Task {
public:
  CameraCreateReplyTask(CameraManage* manage) :manager_(manage) {}
  virtual void DoEvent() {
    manager_->InitCompleteTrigger();
  }
private:
  CameraManage* manager_;
};

///////////////////////CameraConnectReplyTask///////////////////
//the reply from the camera task that invoke the connect task
class CameraConnectReplyTask : public ThreadMessageDispatcher::Task {
public:
  CameraConnectReplyTask(CameraManage* manager) : manager_(manager) {}
  void  SetStatus(IRCameraStatusCode code) { status_code = code; }
  virtual void DoEvent() {
    if (!manager_->connect_result_observer_)
      return;
    if (status_code == IRCAMERA_OK)
      manager_->connect_result_observer_->OnConnectSuccess();
    else
      manager_->connect_result_observer_->OnConnectFalied(status_code);
  }
private:
  IRCameraStatusCode status_code;
  CameraManage* manager_;
};

///////////////////////CameraConnectTask/////////////////////
//connect to the camera
class CameraConnectTask : public ThreadMessageDispatcher::Task {
public:
  CameraConnectTask(CameraManage* manager) : manager_(manager) {}
  virtual void DoEvent() {
    IRCameraStatusCode code = manager_->camera_info->Connect(_T(" "));
    CameraConnectReplyTask* reply_task =
      static_cast<CameraConnectReplyTask*>(manager_->connect_reply_task);
    reply_task->SetStatus(code);
    manager_->main_dispatcher_->PushTask(reply_task);
  }
private:
  CameraManage* manager_;
};

//////////////////////CameraDisconnectTask//////////////////////////
//disconnect from the camera
class CameraDisconnectTask : public ThreadMessageDispatcher::Task {
public:
  CameraDisconnectTask(CameraManage* manager) :manager_(manager)  {}
  virtual void DoEvent() {
    manager_->camera_info->Disconnect();
  }
private:
  CameraManage* manager_;
};
 
////////////////////CameraUpdateImageTask///////////////////////
class CameraUpdateImageTask : public ThreadMessageDispatcher::Task {
public:
  CameraUpdateImageTask(CameraManage* manager) :manager_(manager)  {}
  void SetImageFilling(IRCameraImageFilling* filling) { image_filling_ = filling; }
  virtual void DoEvent() {
    IRCameraStatusCode code = manager_->camera_info->GetKelvinImage(image_filling_);
    if (code == IRCAMERA_OK) {
      manager_->main_dispatcher_->PushTask(manager_->update_image_reply_task_);
    }
  }
private:
  CameraManage* manager_;
  IRCameraImageFilling* image_filling_;
};
CameraManage::CameraManage() :camera_dispatcher_(NULL)  {
  connect_result_observer_ = NULL;
  camera_status_ = DISCONNECTED;
}

//////////////////////CameraUpdateImageReplyTask/////////////////
class  CameraUpdateImageReplyTask : public ThreadMessageDispatcher::Task {
public:
  CameraUpdateImageReplyTask(CameraManage* manager) :manager_(manager)  {}
  virtual void DoEvent() {
    manager_->UpdateImageTrigger();
  }
private:
  CameraManage* manager_;
};


void CameraManage::Init()
{
  ThreadMessageDispatcherManage* manager = ThreadMessageDispatcherManage::GetInstance();
  camera_dispatcher_ = manager->GetCameraDispatcher();
  main_dispatcher_ = manager->GetMainDispatcher();
  assert(camera_dispatcher_ != NULL);
  
  event_handler_.reset(new CameraManageEventHandler(this));
  create_task = new CameraCreateTask(this);
  connect_task = new CameraConnectTask(this);
  disconnect_task = new CameraDisconnectTask(this);
  update_image_task_ = new CameraUpdateImageTask(this);

  create_reply_task = new CameraCreateReplyTask(this);
  connect_reply_task = new CameraConnectReplyTask(this);
  event_handler_task_ = new CameraEventHandlerTask(this);
  update_image_reply_task_ = new CameraUpdateImageReplyTask(this);

  camera_dispatcher_->PushTask(create_task);
//  create_task->DoEvent();
}


CameraManage::~CameraManage()
{
  if (create_task)  delete create_task;
  if (create_reply_task)  delete create_reply_task;
  if (connect_task) delete connect_task;
  if (disconnect_task)  delete disconnect_task;
  if (connect_reply_task) delete connect_reply_task;
  if (event_handler_task_)  delete event_handler_task_;

  if (update_image_task_) delete update_image_task_;
  if (update_image_reply_task_) delete update_image_reply_task_;
  if (camera_status_ == CONNECTED)
    Disconnect();
//  IRCameraDestroy(camera_info);
}

void CameraManage::Connect(ConnectResultObserver* observer) {
  connect_result_observer_ = observer;
  camera_dispatcher_->PushTask(connect_task);
//  connect_task->DoEvent();
}

void CameraManage::Disconnect(){
//  return IRCameraDisconnect(camera_info);  camera_dispatcher_->PushTask(disconnect_task);
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
  return camera_info->GetErrorString(code);
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

void CameraManage::UpdateKelvinImage(IRCameraImageFilling* img_filling) {
  CameraUpdateImageTask* task = static_cast<CameraUpdateImageTask*>(update_image_task_);
  task->SetImageFilling(img_filling);
  camera_dispatcher_->PushTask(update_image_task_);
}

void CameraManage::UpdateImageTrigger() {
  for (ObserverIterator iter = observers_.begin(); iter != observers_.end(); ++iter) {
    (*iter)->OnImageUpdate();;
  }
}
 