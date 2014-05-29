#pragma once

#include "IRCameraDriver.h"
#include <vector>
#include "ThreadMessageDispatcher.h"
struct IRCameraInfo;

class  CameraConnectTask;
class  CameraDisconnectTask;
class  CameraCreateTask;
class  CameraDestroyTask;

class  CameraCreateReplyTask;
class  CameraDestroyReplyTask;
class  CameraConnectReplyTask;

class  CameraEventHandlerTask;

class  CameraUpdateImageTask;
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

  void GetErrorString(IRCameraStatusCode code, LPTSTR buffer, int len);

  int   GetStatus();

  //notify the camera to get the new image 
  //note: img_filling must be multithread security
  void  UpdateKelvinImage(IRCameraImageFilling* img_filling);
private:
  IRCameraStatusCode GetKelvinImage(IRCameraImageFilling* img_filling);

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

  IRCameraInfo* camera_info;
  typedef std::vector<ConnectStatusObserver*>::iterator  ObserverIterator;
  std::vector<ConnectStatusObserver*>   observers_;

  friend class  CameraConnectTask;
  friend class  CameraDisconnectTask;
  friend class  CameraCreateTask;
  friend class  CameraDestroyTask;

  friend class  CameraCreateReplyTask;
  friend class  CameraDestroyReplyTask;
  friend class  CameraConnectReplyTask;

  friend class  CameraEventHandlerTask;

  friend class  CameraUpdateImageTask;
  friend class  CameraUpdateImageReplyTask;

  ConnectResultObserver*  connect_result_observer_;

  static void StaticEventHandle(int event_type, void* args);
};

