#pragma once
#include "UIlib.h"

namespace camera {
class CameraDevice;
}
class ControlBuilder : public DuiLib::IDialogBuilderCallback
{
public:
  ControlBuilder();
  void Init(camera::CameraDevice* manage);
  ~ControlBuilder();

  virtual DuiLib::CControlUI* CreateControl(LPCTSTR class_name) override;
private:
  camera::CameraDevice*   camera_info;
};