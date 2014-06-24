#include "ControlBuilder.h"
#include  "CameraImageControlUI.h"
#include "TemperatureColorTableUI.h"

/////////////////////////CameraImageControl////////////////////
ControlBuilder::ControlBuilder() {
  camera_info = NULL;
}

void ControlBuilder::Init(camera::CameraDevice* manage) {
  camera_info = manage;
}

ControlBuilder::~ControlBuilder() {
}

DuiLib::CControlUI*  ControlBuilder::CreateControl(LPCTSTR class_name) {
  if (_tcscmp(class_name, _T("CameraImage")) == 0) {
    return new CameraImageControlUI(camera_info);
  }
  else if (_tcscmp(class_name, _T("TemperatureTable")) == 0) {
    return new TemperatureColorTableUI;
  }
  return NULL;
}