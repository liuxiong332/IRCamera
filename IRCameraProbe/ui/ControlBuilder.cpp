#include "ControlBuilder.h"
#include  "CameraImageUI.h"
#include "TemperatureColorTableUI.h"

/////////////////////////CameraImageControl////////////////////
ControlBuilder::ControlBuilder() {
}

ControlBuilder::~ControlBuilder() {
}

DuiLib::CControlUI*  ControlBuilder::CreateControl(LPCTSTR class_name) {
  if (_tcscmp(class_name, _T("CameraImage")) == 0) {
    return new CameraImageUI;
  }
  else if (_tcscmp(class_name, _T("TemperatureTable")) == 0) {
    return new TemperatureColorTableUI;
  }
  return NULL;
}