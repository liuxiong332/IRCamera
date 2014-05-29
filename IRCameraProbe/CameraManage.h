#pragma once

#include "IRCameraDriver.h"
#include <vector>
#include "ThreadMessageDispatcher.h"
#include <memory>

class  CameraManageEventHandler;
class  CameraConnectTask;
class  CameraDisconnectTask;
class  CameraCreateTask;
class  CameraDestroyTask;
class  CameraUpdateImageTask;

//the task run in the main thread
class  CameraEventHandlerTask;
class  CameraCreateReplyTask;
class  CameraDestroyReplyTask;
class  CameraConnectReplyTask;
class  CameraUpdateImageReplyTask;

class CameraManage
{
public:
  enum ConnectStatus {
    CONNECTED,
    DISCONNECTED
  };

  class ConnectStatusObserver {
  public:
    virtual void  OnInitCamera() = 0;
    virtual void  OnConnect() = 0;
    virtual void  OnDisconnect() = 0;
    virtual void  OnImageUpdate() = 0;
    virtual ~ConnectStatusObserver()  {}
  };

  //when connect to the camera, the observer return the connect result
  class ConnectResultObserver {
  public: 
    virtual void  OnConnectSuccess() = 0;
    virtual void  OnConnectFalied(IRCameraStatusCode code) = 0;
    virtual ~ConnectResultObserver()  {}
  };

  CameraManage();
  ~CameraManage();
  void Init();

  void Connect(ConnectResultObserver* observer);
  void Disconnect();

  int  GetImageWidth() const;
  int  GetImageHeight() const;

  void  AddConnectStatusObserver(ConnectStatusObserver* observer);
  void  RemoveConnectStatusObserver(ConnectStatusObserver* observer);

  TString GetErrorString(IRCameraStatusCode code);

  int   GetStatus();

  //notify the camera to get the new image 
  //note: img_filling must be multithread security
  void  UpdateKelvinImage(IRCameraImageFilling* img_filling);
private:

  void  InitCompleteTrigger();
  void  ConnectedTrigger();
  void  DisconnectTrigger();
  void  UpdateImageTrigger();

  typedef ThreadMessageDispatcher::Task  Task;
  Task*  create_task;
  Task*  create_reply_task;
  Task*  connect_task;
  Task*  connect_reply_task;
  Task*  disconnect_task;
  Task* event_handler_task_;

  Task*  update_image_task_;
  Task*  update_image_reply_task_;

  int  image_width_, image_height_;
  ConnectStatus   camera_status_;

  ThreadMessageDispatcher*  camera_dispatcher_;
  ThreadMessageDispatcher*  main_dispatcher_;

  IRCameraDevice* camera_info;
  typedef std::vector<ConnectStatusObserver*>::iterator  ObserverIterator;
  std::vector<ConnectStatusObserver*>   observers_;

  std::unique_ptr<IRCameraDevice::IRCameraEventHandler> event_handler_;
  // the camera event handler that register into the camera interface, run in camera thread
  friend class  CameraManageEventHandler;
  friend class  CameraConnectTask;
  friend class  CameraDisconnectTask;
  friend class  CameraCreateTask;
  friend class  CameraDestroyTask;
  friend class  CameraUpdateImageTask;

  //the task run in the main thread
  friend class  CameraEventHandlerTask;
  friend class  CameraCreateReplyTask;
  friend class  CameraDestroyReplyTask;
  friend class  CameraConnectReplyTask;
  friend class  CameraUpdateImageReplyTask;

  ConnectResultObserver*  connect_result_observer_;
};

