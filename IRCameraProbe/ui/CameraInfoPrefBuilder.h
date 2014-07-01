#pragma once

class CameraInfoUI;

class CameraInfoPrefBuilder {
public:
  //create a new CameraInfoUI to describe the camera info
  virtual CameraInfoUI* CreateCameraInfoUI() = 0;
  virtual ~CameraInfoPrefBuilder()  {}
};