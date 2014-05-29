#pragma once
#include "UIlib.h"
#include <windows.h>

namespace DuiLib {
  class CControlUI;
}
class CameraManage;

class CameraImageControl: public DuiLib::IDialogBuilderCallback
{
public:
  CameraImageControl();
  void Init(CameraManage* manage);
  ~CameraImageControl();

  virtual DuiLib::CControlUI* CreateControl(LPCTSTR class_name) override;
private:
  CameraManage*   camera_info;
};

