#include "CameraImageContainerDeviceLinker.h"
#include "core/CameraDevice.h"

namespace camera {
  
CameraImageContainerDeviceLinker::CameraImageContainerDeviceLinker()
    : container_ui_(NULL),
      camera_device_(NULL)  {
}
void CameraImageContainerDeviceLinker::Init(CameraDevice* device, CameraImageContainerUI* ui) {
  container_ui_ = ui;
  camera_device_ = device;
}
}
