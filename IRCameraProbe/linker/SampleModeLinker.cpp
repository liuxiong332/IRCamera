#include "SampleModeLinker.h"
#include "pref/SampleModePref.h"
#include "ui/SampleModePrefUI.h"

namespace camera {
 
SampleModeLinker::SampleModeLinker(SampleModePrefUI* ui) {
  sample_mode_ui_ = ui;
  SampleModeChanged();
}

void SampleModeLinker::SampleModeChanged() {
  SampleModePref* pref = SampleModePref::GetInstance();
  pref->SetSampleMode(sample_mode_ui_->GetSampleMode());
  pref->SetSampleInterval(sample_mode_ui_->GetSampleInterval());
}
}