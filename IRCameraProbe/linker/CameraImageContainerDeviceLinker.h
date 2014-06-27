#pragma once

class CameraImageContainerUI;
namespace camera {
class CameraDevice;
class CameraImageContainerDeviceLinker {
public:
  CameraImageContainerDeviceLinker();

  void Init(CameraDevice* device, CameraImageContainerUI* ui);

  CameraImageContainerUI* GetContainerUI();
private:
  CameraImageContainerUI* container_ui_;
  CameraDevice* camera_device_;
};

}