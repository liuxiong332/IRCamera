#pragma once
#include "IRCameraDevice.h"

class CameraImageBuffer;
namespace camera {

class CameraDeviceObserver {
public:
  //when the camera has init completely
  virtual void  OnInitCamera() = 0;
  //when connect to the camera successfully
  virtual void  OnConnect() = 0;
  //when the host has disconnect from the camera
  virtual void  OnDisconnect() = 0;
  //when the image has update
  virtual void  OnImageUpdate(CameraImageBuffer*) = 0;
  virtual void  OnImageUpdateFail(IRCameraStatusCode code) = 0;
  virtual ~CameraDeviceObserver()  {}
};
}