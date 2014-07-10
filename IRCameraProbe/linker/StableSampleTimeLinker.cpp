#include "StableSampleTimeLinker.h"
#include "pref/StableSampleTimePref.h"
#include "ui/StableSampleTimePrefUI.h"

namespace camera {
StableSampleTimeLinker::StableSampleTimeLinker(StableSampleTimePrefUI* ui) {
  pref_ui_ = ui;
  StableSampleTimeChanged();
}

void StableSampleTimeLinker::StableSampleTimeChanged() {
  StableSampleTimePref* pref = StableSampleTimePref::GetInstance();
  pref->SetSampleHour(pref_ui_->GetHour());
}
}