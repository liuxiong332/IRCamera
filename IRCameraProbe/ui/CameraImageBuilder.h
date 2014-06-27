#pragma once
class CameraImageContainerUI;

class CameraImageBuilder {
public:
  virtual CameraImageContainerUI* CreateCameraImageContainerUI() = 0;
  virtual ~CameraImageBuilder() {}
};