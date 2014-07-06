#include "CameraPrefLinker.h"
#include "CameraInfoListLinker.h"
#include "SampleModeLinker.h"
#include "TempThresholdLinker.h"

#include "ui/CameraPrefLayoutUI.h"
#include "ui/CameraInfoPrefLayout.h"
#include "ui/TempThresholdPrefUI.h"


namespace camera {
void CameraPrefLinker::Init(CameraPrefLayoutUI* layout) {
  layout->SetObserver(this);
  layout_ui_ = layout;
  camera_info_linker_.reset(new CameraInfoListLinker);
  camera_info_linker_->Init(layout->GetCameraInfoPrefLayout()->GetCameraInfoList());

  sample_mode_linker_.reset(new SampleModeLinker(layout->GetSampleModePrefUI()));
  temp_threshold_linker_.reset(new TempThresholdLinker(layout->GetTempThresholdPrefUI()));
  OnBackButtonClicked();    //make the setup valid
}

void CameraPrefLinker::OnBackButtonClicked() {
  //when the back button clicked, we will make the camera info preference available
  camera_info_linker_->CameraInfoUIChanged(); 
  sample_mode_linker_->SampleModeChanged();
  temp_threshold_linker_->TempThresholdChanged();
}
 
}