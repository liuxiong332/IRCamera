#include "CameraInfoPrefLayout.h"
#include "CommonUIOperator.h"
#include "CameraInfoListUI.h"
#include "CameraInfoUI.h"
#include "UIlib.h"

void CameraInfoPrefLayout::Init(DuiLib::CContainerUI* container_ui, CameraInfoPrefBuilder* builder) {
  container_ui_ = container_ui;
  add_button_ = CommonUIOperator::FindSubButtonByName(container_ui, _T("add_btn"));
  add_button_->OnNotify += DuiLib::MakeDelegate(this, &CameraInfoPrefLayout::OnAddButtonClick);

  edit_button_ = CommonUIOperator::FindSubButtonByName(container_ui, _T("edit_btn"));
  edit_button_->OnNotify += DuiLib::MakeDelegate(this, &CameraInfoPrefLayout::OnEditButtonClick);

  remove_button_ = CommonUIOperator::FindSubButtonByName(container_ui, _T("remove_btn"));
  remove_button_->OnNotify += DuiLib::MakeDelegate(this, &CameraInfoPrefLayout::OnRemoveButtonClick);
  remove_button_->SetVisible(false);

  DuiLib::CContainerUI* list_ui = CommonUIOperator::FindSubContainerByName(container_ui, _T("camera_info_list"));

  camera_info_list_.reset(new CameraInfoListUI);
  camera_info_list_->Init(list_ui, builder);
}

DuiLib::CContainerUI* CameraInfoPrefLayout::GetUnderlyingUI() {
  return container_ui_;
}

bool CameraInfoPrefLayout::OnAddButtonClick(void* param) {
  DuiLib::TNotifyUI* notify = static_cast<DuiLib::TNotifyUI*>(param);
  if (notify->sType == _T("click")) {
    camera_info_list_->AddCameraInfo();
    return true;
  }
  return false;
}

bool CameraInfoPrefLayout::OnRemoveButtonClick(void* param) {
  DuiLib::TNotifyUI* notify = static_cast<DuiLib::TNotifyUI*>(param);
  if (notify->sType == _T("click")) {
    camera_info_list_->RemoveAllCheckedCameraInfo();
    camera_info_list_->EnableCheckbox(false);

    add_button_->SetVisible(true);
    edit_button_->SetVisible(true);
    remove_button_->SetVisible(false);
    return true;
  }
  return false;
}

bool CameraInfoPrefLayout::OnEditButtonClick(void* param) {
  DuiLib::TNotifyUI* notify = static_cast<DuiLib::TNotifyUI*>(param);
  if (notify->sType == _T("click")) {
    camera_info_list_->EnableCheckbox(true);

    add_button_->SetVisible(false);
    edit_button_->SetVisible(false);
    remove_button_->SetVisible(true);
    return true;
  }
  return false;
}