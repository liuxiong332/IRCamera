#include "CameraInfoListLinker.h"
#include "ui/CameraInfoListUI.h"
#include "ui/CameraInfoUI.h"
#include "pref/CameraInfoPref.h"

namespace camera {
void CameraInfoListLinker::Init(CameraInfoListUI* ui) {
  list_ui_ = ui;
  CameraInfoUIChanged();
}

void CameraInfoListLinker::CameraInfoUIChanged() {
  CameraInfoPref* pref = CameraInfoPref::GetInstance();
  pref->Clear();
  for (int i = 0; i < list_ui_->GetCameraInfoCount(); ++i) {
    CameraInfoUI* camera_info = list_ui_->GetCameraInfo(i);
    pref->PushBack(camera_info->GetIPAddr(), camera_info->GetName());
  }
  pref->CameraInfoPrefChanged();
}
}