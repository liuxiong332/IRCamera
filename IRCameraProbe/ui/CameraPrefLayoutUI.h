#pragma once
#include <memory>

namespace DuiLib {
class CContainerUI;
class CButtonUI;
}

class CameraInfoPrefLayout;
class SampleModePrefUI;

class TabLayoutSwitcher;
class CameraPrefLayoutUIObserver;
class CameraInfoPrefBuilder;
class TempThresholdPrefUI;

class CameraPrefLayoutUI {
public:
  CameraPrefLayoutUI();

  void Init(DuiLib::CContainerUI* container_ui, TabLayoutSwitcher* switcher, CameraInfoPrefBuilder* builder);

  void SetObserver(CameraPrefLayoutUIObserver* observer) {
    observer_ = observer;
  }

  CameraInfoPrefLayout* GetCameraInfoPrefLayout();
  SampleModePrefUI*   GetSampleModePrefUI() {
    return sample_mode_pref_ui_.get();
  }

  TempThresholdPrefUI*  GetTempThresholdPrefUI() {
    return temp_threshold_pref_ui_.get();
  }

private:
  bool  OnBackButtonClick(void* param);

  TabLayoutSwitcher*  layout_switcher_;
  CameraPrefLayoutUIObserver* observer_;

  DuiLib::CButtonUI*  back_button_;
  std::unique_ptr<CameraInfoPrefLayout> camera_info_pref_layout_;
  std::unique_ptr<SampleModePrefUI>     sample_mode_pref_ui_;
  std::unique_ptr<TempThresholdPrefUI>  temp_threshold_pref_ui_;
};