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
    ptr->camera_info_ = IRCameraDevice::Create(); // IRCameraDevice::CreateForTest();
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
    *code = ptr->camera_info_->Connect(ptr->GetIPAddr());
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
  const static TCHAR* kErrorStrings[] = {
    _T("OK, No Error"), _T("Device not present"),
    _T("Device busy"), _T("Device driver missing"),
    _T("Device driver must be updated"), _T("Failed to load device firmware"),
    _T("Failed to configure device. Resource Conflict"), _T("Unknown error"),
    _T("Service Control Manager error"), _T("Failed to establish a camera control connection"),
    _T("Control connection closed"), _T("Cannot allocate image buffer"),
    _T("invalid image"), _T("Device hardware error"),
    _T("Timeout waiting for image"), _T("Camera configuration error"),
    _T("Image acquision aborted"), _T("Image source is not initiated")
  };
  int code_index = static_cast<int>(code);
  if (code_index<0 || code_index>ARRAYSIZE(kErrorStrings)) {
    return _T("Unknown Error");
  }
  return kErrorStrings[code_index];
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
    } else {
      ptr->main_message_loop_->PushTask([ptr, code] {
        ptr->UpdateFailedTrigger(code);
      });
    }
  });
}

void CameraDevice::UpdateImageTrigger(CameraImageBuffer* buffer) {
  for (ObserverIterator iter = observers_.begin(); iter != observers_.end(); ++iter) {
    (*iter)->OnImageUpdate(buffer); 
  }
}

void CameraDevice::UpdateFailedTrigger(IRCameraStatusCode code) {
  std::for_each(observers_.begin(), observers_.end(), [code](CameraDeviceObserver* observer) {
    observer->OnImageUpdateFail(code);
  });
}
 
}