#include "SampleModeLinker.h"
#include "pref/CameraPref.h"
#include "ui/SampleModePrefUI.h"

namespace camera {
 
SampleModeLinker::SampleModeLinker(SampleModePrefUI* ui) {
  sample_mode_ui_ = ui;
  SampleModePref* pref = CameraPref::GetInstance()->GetSampleMode();
  sample_mode_ui_->SetSampleMode(pref->GetSampleMode());
  sample_mode_ui_->SetSampleInterval(pref->GetSampleInterval());
}

void SampleModeLinker::SampleModeChanged() {
  SampleModePref* pref = CameraPref::GetInstance()->GetSampleMode();
  pref->SetSampleMode(sample_mode_ui_->GetSampleMode());
  pref->SetSampleInterval(sample_mode_ui_->GetSampleInterval());
}
}