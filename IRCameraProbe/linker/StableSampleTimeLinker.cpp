#include "StableSampleTimeLinker.h"
#include "pref/CameraPref.h"
#include "ui/StableSampleTimePrefUI.h"

namespace camera {
StableSampleTimeLinker::StableSampleTimeLinker(StableSampleTimePrefUI* ui) {
  pref_ui_ = ui;
  StableSampleTimePref* pref = CameraPref::GetInstance()->GetStableSampleTime();
  pref_ui_->SetHour(pref->GetSampleHour());
}

void StableSampleTimeLinker::StableSampleTimeChanged() {
  StableSampleTimePref* pref = CameraPref::GetInstance()->GetStableSampleTime();
  pref->SetSampleHour(pref_ui_->GetHour());
}
}