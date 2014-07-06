#pragma once
#include "UIlib.h"
 
class CanEditButtonUICreator;
class ControlBuilder : public DuiLib::IDialogBuilderCallback
{
public:
  ControlBuilder();
  ~ControlBuilder();
  void SetCanEditButtonUICreator(CanEditButtonUICreator* creator);

  virtual DuiLib::CControlUI* CreateControl(LPCTSTR class_name) override;
private:
  CanEditButtonUICreator* edit_button_ui_creator_;
};