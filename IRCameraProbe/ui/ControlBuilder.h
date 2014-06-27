#pragma once
#include "UIlib.h"
 
class ControlBuilder : public DuiLib::IDialogBuilderCallback
{
public:
  ControlBuilder();
  ~ControlBuilder();

  virtual DuiLib::CControlUI* CreateControl(LPCTSTR class_name) override;
};