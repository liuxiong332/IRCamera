#pragma once

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
  virtual void  OnImageUpdate() = 0;
  virtual ~CameraDeviceObserver()  {}
};
}