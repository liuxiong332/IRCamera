#include "TempThresholdLinker.h"
#include "ui/TempThresholdPrefUI.h"
#include "pref/CameraPref.h"

namespace camera {
TempThresholdLinker::TempThresholdLinker(TempThresholdPrefUI* ui) {
  pref_ui_ = ui;
  TempThresholdPref* pref = CameraPref::GetInstance()->GetTempThreshold();
  pref_ui_->SetStableMaxTemp(pref->GetStableMaxTemp());
  pref_ui_->SetStableDeltaTemp(pref->GetStableDeltaTemp());
  pref_ui_->SetUnstableMaxTemp(pref->GetUnstableMaxTemp());
  pref_ui_->SetUnstableMaxDeltaTemp(pref->GetUnstableMaxDeltaTemp());
}

void TempThresholdLinker::TempThresholdChanged() {
  TempThresholdPref* pref = CameraPref::GetInstance()->GetTempThreshold();
  pref->stable_max_temp_ = pref_ui_->GetStableMaxTemp();
  pref->stable_delta_temp_ = pref_ui_->GetStableDeltaTemp();

  pref->unstable_max_delta_temp_ = pref_ui_->GetUnstableMaxDeltaTemp();
  pref->unstable_max_temp_ = pref_ui_->GetUnstableMaxTemp();
}
}