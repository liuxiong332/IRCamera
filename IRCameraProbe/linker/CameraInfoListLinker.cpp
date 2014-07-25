#include "CameraInfoListLinker.h"
#include "ui/CameraInfoListUI.h"
#include "ui/CameraInfoUI.h"
#include "pref/CameraPref.h"

namespace camera {
void CameraInfoListLinker::Init(CameraInfoListUI* ui) {
  list_ui_ = ui;
  CameraInfoPref* pref = CameraPref::GetInstance()->GetCameraInfoPref();
  for (int i = 0; i < pref->GetCameraCount(); ++i) {
    const CameraInfoPref::CameraInfo& info = pref->GetCameraAt(i);
    list_ui_->AddCameraInfo(info.ip_addr, info.name);     
  }
}

void CameraInfoListLinker::CameraInfoUIChanged() {
  CameraInfoPref* pref = CameraPref::GetInstance()->GetCameraInfoPref();
  pref->Clear();
  for (int i = 0; i < list_ui_->GetCameraInfoCount(); ++i) {
    CameraInfoUI* camera_info = list_ui_->GetCameraInfo(i);
    pref->PushBack(camera_info->GetIPAddr(), camera_info->GetName());
  }
  pref->CameraInfoPrefChanged();
}
}