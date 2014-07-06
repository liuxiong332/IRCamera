#include "CameraPrefLinker.h"
#include "CameraInfoListLinker.h"
#include "SampleModeLinker.h"

#include "ui/CameraPrefLayoutUI.h"
#include "ui/CameraInfoPrefLayout.h"


namespace camera {
void CameraPrefLinker::Init(CameraPrefLayoutUI* layout) {
  layout->SetObserver(this);
  layout_ui_ = layout;
  camera_info_linker_.reset(new CameraInfoListLinker);
  camera_info_linker_->Init(layout->GetCameraInfoPrefLayout()->GetCameraInfoList());

  sample_mode_linker_.reset(new SampleModeLinker(layout->GetSampleModePrefUI()));
  OnBackButtonClicked();    //make the setup valid
}

void CameraPrefLinker::OnBackButtonClicked() {
  //when the back button clicked, we will make the camera info preference available
  camera_info_linker_->CameraInfoUIChanged(); 
  sample_mode_linker_->SampleModeChanged();
}
 
}