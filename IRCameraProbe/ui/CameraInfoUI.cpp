#include "CameraInfoUI.h"
#include "CommonUIOperator.h"

void CameraInfoUI::Init(DuiLib::CContainerUI* container_ui) {
  name_button_ = CommonUIOperator::FindSubButtonByName(container_ui, _T("camera_name_btn"));
  name_edit_ = CommonUIOperator::FindSubRichEditByName(container_ui, _T("camera_name_edit")); 
  name_ui_transform_.Init(name_button_, name_edit_);

  ipaddr_button_ = CommonUIOperator::FindSubButtonByName(container_ui, _T("camera_ipaddr_btn"));
  ipaddr_edit_ = CommonUIOperator::FindSubRichEditByName(container_ui, _T("camera_ipaddr_edit"));
  ipaddr_ui_transform_.Init(ipaddr_button_, ipaddr_edit_);
}

LPCTSTR CameraInfoUI::GetIPAddr() const {
  return name_button_->GetText();
}
LPCTSTR CameraInfoUI::GetName() const {
  return ipaddr_button_->GetText();
}