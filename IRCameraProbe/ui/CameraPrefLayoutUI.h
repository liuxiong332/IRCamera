#pragma once
#include <memory>

namespace DuiLib {
class CContainerUI;
class CButtonUI;
}

class CameraInfoPrefLayout;
class TabLayoutSwitcher;
class CameraPrefLayoutUIObserver;
class CameraInfoPrefBuilder;

class CameraPrefLayoutUI {
public:
  CameraPrefLayoutUI();

  void Init(DuiLib::CContainerUI* container_ui, TabLayoutSwitcher* switcher, CameraInfoPrefBuilder* builder);

  void SetObserver(CameraPrefLayoutUIObserver* observer) {
    observer_ = observer;
  }

  CameraInfoPrefLayout* GetCameraInfoPrefLayout();
private:
  bool  OnBackButtonClick(void* param);

  TabLayoutSwitcher*  layout_switcher_;
  CameraPrefLayoutUIObserver* observer_;

  DuiLib::CButtonUI*  back_button_;
  std::unique_ptr<CameraInfoPrefLayout> camera_info_pref_layout_;
};