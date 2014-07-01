#pragma once

namespace DuiLib {
class CButtonUI;
class CControlUI;
}

//the utility to provide the transformation between
//the button and the edit control
class ButtonEditUITransform {
public:
  void Init(DuiLib::CButtonUI* button, DuiLib::CControlUI* control_ui);
private:
  bool OnButtonClicked(void* param);
  bool OnControlKillFocus(void* param);

  DuiLib::CButtonUI*  button_ui_;
  DuiLib::CControlUI* control_ui_;
};