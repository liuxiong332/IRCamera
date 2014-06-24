#pragma once

#include "IRCameraDevice.h"
#include "CameraDeviceObserver.h"
#include <vector>
#include <memory>

namespace camera {

class MessageLoop;
class CameraManageEventHandler;

class CameraDevice
{
public:
  enum ConnectStatus {
    CONNECTED,
    DISCONNECTED
  };

  //when connect to the camera, the observer return the connect result
  class ConnectResultObserver {
  public: 
    virtual void  OnConnectSuccess() = 0;
    virtual void  OnConnectFalied(IRCameraStatusCode code) = 0;
    virtual ~ConnectResultObserver()  {}
  };

  CameraDevice();
  ~CameraDevice();

  //set and get the name of the camera
  void SetName(const TString& name) { camera_name_ = name;   }
  const TString& GetName() const    { return camera_name_;   }
  //set and get the ip address of camera
  void SetIPAddr(const TString& ip_addr)  { camera_ip_addr_ = ip_addr;   }
  const TString& GetIPAddr() const  { return camera_ip_addr_;  }

  void Init();

  void Connect(ConnectResultObserver* observer);
  void Disconnect();

  int  GetImageWidth() const;
  int  GetImageHeight() const;

  void  AddConnectStatusObserver(CameraDeviceObserver* observer);
  void  RemoveConnectStatusObserver(CameraDeviceObserver* observer);

  TString GetErrorString(IRCameraStatusCode code);

  int   GetStatus();

  //notify the camera to get the new image 
  //note: img_filling must be multithread security
  void  UpdateKelvinImage(IRCameraImageFilling* img_filling);
private:
  friend class CameraManageEventHandler;
  //the observer to observe the camera event in the camera thread
  void  CameraEventHandlerInCameraThread(IRCameraEvent event_type);

  void  InitCompleteTrigger();
  void  ConnectedTrigger();
  void  DisconnectTrigger();
  void  UpdateImageTrigger();

  TString camera_name_;
  TString camera_ip_addr_;

  int  image_width_, image_height_;
  ConnectStatus   camera_status_;

  IRCameraDevice* camera_info_;
  MessageLoop*  main_message_loop_;
  MessageLoop*  camera_message_loop_;

  typedef std::vector<CameraDeviceObserver*>::iterator  ObserverIterator;
  std::vector<CameraDeviceObserver*>   observers_;

  std::unique_ptr<IRCameraDevice::IRCameraEventHandler> camera_event_handler_in_camera_thread_;
};

}