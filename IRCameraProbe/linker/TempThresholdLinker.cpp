#include "TempThresholdLinker.h"
#include "ui/TempThresholdPrefUI.h"
#include "pref/TempThresholdPref.h"

namespace camera {
TempThresholdLinker::TempThresholdLinker(TempThresholdPrefUI* ui) {
  pref_ui_ = ui;
}

void TempThresholdLinker::TempThresholdChanged() {
  TempThresholdPref* pref = TempThresholdPref::GetInstance();
  pref->stable_max_temp_ = pref_ui_->GetStableMaxTemp();
  pref->unstable_aver_delta_temp_ = pref_ui_->GetUnstableAverDeltaTemp();
  pref->unstable_aver_temp_ = pref_ui_->GetUnstableAverTemp();
  pref->unstable_max_delta_temp_ = pref_ui_->GetUnstableMaxDeltaTemp();
  pref->unstable_max_temp_ = pref_ui_->GetUnstableMaxTemp();
}
}