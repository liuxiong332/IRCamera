#include "ControlBuilder.h"
#include  "CameraImageUI.h"
#include "TemperatureColorTableUI.h"
#include "CanEditButtonUICreator.h"
#include "CanEditButtonUI.h"
#include "CameraImageRealTimeUI.h"

/////////////////////////CameraImageControl////////////////////
ControlBuilder::ControlBuilder():edit_button_ui_creator_(NULL) {
}

ControlBuilder::~ControlBuilder() {
}

void ControlBuilder::SetCanEditButtonUICreator(CanEditButtonUICreator* creator) {
  edit_button_ui_creator_ = creator;
}
DuiLib::CControlUI*  ControlBuilder::CreateControl(LPCTSTR class_name) {
  if (_tcscmp(class_name, _T("CameraImage")) == 0) {
    return new CameraImageUI;
  } else if (_tcscmp(class_name, _T("TemperatureTable")) == 0) {
    return new TemperatureColorTableUI;
  } else if (_tcscmp(class_name, _T("CanEditButton")) == 0) {
    if (edit_button_ui_creator_)
      return edit_button_ui_creator_->CreateCanEditButtonUI();
  } else if (_tcscmp(class_name, _T("CameraImageRealTime")) == 0) {
    return new CameraImageRealTimeUI;
  }
  return NULL;
}