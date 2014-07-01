#include "CameraInfoListUI.h"
#include "UIlib.h"

void CameraInfoListUI::Init(DuiLib::CContainerUI* ui) {
  info_list_ui_ = ui;
}

int CameraInfoListUI::GetCameraInfoCount() const {
  return info_list_ui_->GetCount();
}

CameraInfoUI* CameraInfoListUI::GetCameraInfo(int i) {
  return NULL;
}