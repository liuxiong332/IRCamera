#include "StableSampleTimePrefUI.h"
#include "CommonUIOperator.h"
#include "common/StringUtility.h"

StableSampleTimePrefUI::StableSampleTimePrefUI(DuiLib::CContainerUI* container_ui) {
  time_ui_ = CommonUIOperator::FindSubCanEditButtonByName(container_ui, _T("stable_sample_time_btn"));
}

int StableSampleTimePrefUI::GetHour() {
  return wcstol(time_ui_->GetButtonText().c_str(), NULL, 10);
}

void StableSampleTimePrefUI::SetHour(int hour) {
  time_ui_->SetButtonText(StringUtility::IntToString(hour).c_str());
}