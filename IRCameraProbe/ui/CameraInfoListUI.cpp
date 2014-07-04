#include "CameraInfoListUI.h"
#include "CommonUIOperator.h"
#include "CameraInfoPrefBuilder.h"
#include "CameraInfoUI.h"
#include "UIlib.h"
#include <algorithm>

void CameraInfoListUI::Init(DuiLib::CContainerUI* ui, CameraInfoPrefBuilder* builder) {
  info_list_ui_ = ui;
  camera_info_builder_ = builder;
  edit_hide_control_ = CommonUIOperator::FindSubControlByName(info_list_ui_, _T("edit_hide_control"));
  AddCameraInfo();
}

int CameraInfoListUI::GetCameraInfoCount() const {
  return camera_info_list_.size();
}

CameraInfoUI* CameraInfoListUI::GetCameraInfo(int i) {
  return camera_info_list_[i].get();
}

void CameraInfoListUI::RemoveAllCheckedCameraInfo() {
  auto end_iter = std::remove_if(camera_info_list_.begin(), camera_info_list_.end(), [this](const CameraInfoUIPtr& ui) {
    bool is_checked = ui->IsChecked();
    if (is_checked) info_list_ui_->Remove(ui->GetUnderlyingUI());   //first delete the ui from the list
    return is_checked;
  });
  camera_info_list_.erase(end_iter, camera_info_list_.end());
}

void CameraInfoListUI::AddCameraInfo() {
  CameraInfoUI* camera_info_ui = camera_info_builder_->CreateCameraInfoUI();
  info_list_ui_->Add(camera_info_ui->GetUnderlyingUI());
  camera_info_list_.push_back(CameraInfoUIPtr(camera_info_ui));
}

void CameraInfoListUI::EnableCheckbox(bool is_enable) {
  edit_hide_control_->SetVisible(is_enable);
  //enable or disable all the camera info checkbox
  std::for_each(camera_info_list_.begin(), camera_info_list_.end(), [is_enable](const CameraInfoUIPtr& ui) {
    ui->EnableCheckbox(is_enable);
  });
}