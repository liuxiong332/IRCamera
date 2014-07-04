#include "CameraPrefLayoutUI.h"
#include "CameraInfoListUI.h"
#include "CameraInfoUI.h"
#include "CommonUIOperator.h"
#include "TabLayoutSwitcher.h"
#include "CameraPrefLayoutUIObserver.h"
#include "CameraInfoPrefLayout.h"
#include "SampleModePrefUI.h"
#include "UIlib.h"
#include <assert.h>

CameraPrefLayoutUI::CameraPrefLayoutUI()
    :observer_(NULL) {
}

void CameraPrefLayoutUI::Init(DuiLib::CContainerUI* container_ui, TabLayoutSwitcher* switcher,
  CameraInfoPrefBuilder* builder) {
  layout_switcher_ = switcher;
  back_button_ = CommonUIOperator::FindSubButtonByName(container_ui, _T("from_setting_back_btn"));
  assert(back_button_ != NULL);
  back_button_->OnNotify += DuiLib::MakeDelegate(this, &CameraPrefLayoutUI::OnBackButtonClick);

  //get the camera info layout
  DuiLib::CContainerUI* camera_info_layout_ui = 
    CommonUIOperator::FindSubContainerByName(container_ui, _T("camera_info_pref_layout"));
  assert(camera_info_layout_ui != NULL);

  camera_info_pref_layout_.reset(new CameraInfoPrefLayout);
  camera_info_pref_layout_->Init(camera_info_layout_ui, builder);

  DuiLib::CContainerUI* sample_mode_ui =
    CommonUIOperator::FindSubContainerByName(container_ui, _T("sample_mode_ui"));
  sample_mode_pref_ui_.reset(new SampleModePrefUI);
  sample_mode_pref_ui_->Init(sample_mode_ui);
}

CameraInfoPrefLayout* CameraPrefLayoutUI::GetCameraInfoPrefLayout() {
  return camera_info_pref_layout_.get();
}

bool CameraPrefLayoutUI::OnBackButtonClick(void* param) {
  DuiLib::TNotifyUI* notify = static_cast<DuiLib::TNotifyUI*>(param);
  if (notify->sType == _T("click")) {
    layout_switcher_->SwitchToContentLayout();
    if (observer_)
      observer_->OnBackButtonClicked();
    return true;
  }
  return false;
}