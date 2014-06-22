#pragma once
#include "UIlib.h"

class CameraManage;
class ControlBuilder : public DuiLib::IDialogBuilderCallback
{
public:
  ControlBuilder();
  void Init(CameraManage* manage);
  ~ControlBuilder();

  virtual DuiLib::CControlUI* CreateControl(LPCTSTR class_name) override;
private:
  CameraManage*   camera_info;
};