#pragma once
#include <memory>
#include "ui/CameraPrefLayoutUIObserver.h"

class CameraPrefLayoutUI;
//camera preference linker is the control layer between the preference layout and the camera info model
namespace camera {

class CameraInfoListLinker;
class SampleModeLinker;
class TempThresholdLinker;
class StableSampleTimeLinker;

class CameraPrefLinker: public CameraPrefLayoutUIObserver {
public:
  void Init(CameraPrefLayoutUI* layout);
private:
  virtual void OnBackButtonClicked() override;

  CameraPrefLayoutUI*   layout_ui_;
  std::unique_ptr<TempThresholdLinker>  temp_threshold_linker_;
  std::unique_ptr<CameraInfoListLinker> camera_info_linker_;
  std::unique_ptr<SampleModeLinker>   sample_mode_linker_;
  std::unique_ptr<StableSampleTimeLinker> stable_sample_time_linker_;
};
}