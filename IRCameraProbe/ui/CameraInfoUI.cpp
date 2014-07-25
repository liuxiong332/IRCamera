#include "CameraInfoUI.h"
#include "CommonUIOperator.h"

void CameraInfoUI::Init(DuiLib::CContainerUI* container_ui) {
  container_ui_ = container_ui;
  checkbox_ = CommonUIOperator::FindSubOptionByName(container_ui_, _T("camera_info_checkbox"));
  name_button_ = CommonUIOperator::FindSubButtonByName(container_ui, _T("camera_name_btn"));
  name_edit_ = CommonUIOperator::FindSubRichEditByName(container_ui, _T("camera_name_edit")); 
  name_ui_transform_.Init(name_button_, name_edit_);

  ipaddr_button_ = CommonUIOperator::FindSubButtonByName(container_ui, _T("camera_ipaddr_btn"));
  ipaddr_edit_ = CommonUIOperator::FindSubRichEditByName(container_ui, _T("camera_ipaddr_edit"));
  ipaddr_ui_transform_.Init(ipaddr_button_, ipaddr_edit_);
}

bool CameraInfoUI::IsChecked() const {
  return checkbox_->IsSelected();
}

void CameraInfoUI::EnableCheckbox(bool is_enable) {
  checkbox_->SetVisible(is_enable);
}

TString CameraInfoUI::GetIPAddr() const {
  return TString(ipaddr_button_->GetText());
}
TString CameraInfoUI::GetName() const {
  return TString(name_button_->GetText());
}

void CameraInfoUI::SetIPAddr(const TString& str) {
  ipaddr_button_->SetText(str.c_str());
}
void CameraInfoUI::SetName(const TString& str) {
  name_button_->SetText(str.c_str());
}