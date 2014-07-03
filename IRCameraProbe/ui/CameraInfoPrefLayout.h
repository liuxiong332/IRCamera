#pragma once
#include <memory>

namespace DuiLib {
class CContainerUI;
class CButtonUI;
class CControlUI;
}
class CameraInfoPrefBuilder;
class CameraInfoListUI;
class CameraInfoPrefLayout {
public:
  void Init(DuiLib::CContainerUI* container_ui, CameraInfoPrefBuilder* builder);

  CameraInfoListUI*   GetCameraInfoList() {
    return camera_info_list_.get();
  }
  DuiLib::CContainerUI* GetUnderlyingUI();
private:
  bool OnAddButtonClick(void* param);
  bool OnRemoveButtonClick(void* param);
  bool OnEditButtonClick(void* param);

  DuiLib::CContainerUI* container_ui_;

  DuiLib::CButtonUI*  add_button_;
  DuiLib::CButtonUI*  edit_button_;
  DuiLib::CButtonUI*  remove_button_;

  std::unique_ptr<CameraInfoListUI> camera_info_list_;
};