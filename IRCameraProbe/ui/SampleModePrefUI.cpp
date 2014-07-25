#include "SampleModePrefUI.h"
#include "CommonUIOperator.h"
#include "IRCameraBasic.h"
#include "common/StringUtility.h"
#include <algorithm>
#include <regex>

const static TCHAR* kScaleStr[] = { _T("s"), _T("m"), _T("h"), _T("d") };
#define arraysize(array_name) sizeof(array_name)/sizeof(array_name[0])

int  ScaleStrToIndex(const TString& str) {
  const TCHAR** end_iter = kScaleStr + arraysize(kScaleStr);
  const TCHAR** iter = std::find(kScaleStr, end_iter, str);
  if (iter != end_iter) {
    return  iter - kScaleStr;
  }
  return 0;
}

void SampleModePrefUI::Init(DuiLib::CContainerUI* ui) {
  container_ui_ = ui;
  auto_sample_option_ = CommonUIOperator::FindSubOptionByName(ui, _T("auto_mode_option"));
  auto_sample_option_->Selected(true);
  manual_sample_option_ = CommonUIOperator::FindSubOptionByName(ui, _T("manual_mode_option"));

  sample_interval_button_ = CommonUIOperator::FindSubButtonByName(ui, _T("sample_interval_btn"));
  sample_interval_button_->OnNotify += DuiLib::MakeDelegate(this, &SampleModePrefUI::OnButtonClicked);

  sample_interval_ui_ = CommonUIOperator::FindSubContainerByName(ui, _T("sample_interval_ui"));

  sample_interval_edit_ = CommonUIOperator::FindSubRichEditByName(ui, _T("sample_interval_edit"));
  sample_interval_combo_ = CommonUIOperator::FindSubComboByName(ui, _T("sample_interval_combo"));
  SyncButtonToEdit();

  DuiLib::CButtonUI*  ok_button = CommonUIOperator::FindSubButtonByName(ui, _T("sample_interval_ok_btn"));
  ok_button->OnNotify += DuiLib::MakeDelegate(this, &SampleModePrefUI::OnSampleIntervalEditOKClicked);
}


camera::SampleMode  SampleModePrefUI::GetSampleMode() const {
  if (auto_sample_option_->IsSelected())
    return camera::AUTO_MODE;
  return camera::MANUAL_MODE;
}

void SampleModePrefUI::SetSampleMode(camera::SampleMode mode) {
  if (mode == camera::AUTO_MODE)
    auto_sample_option_->Selected(true);
  else
    auto_sample_option_->Selected(false);
}

TimeDelta SampleModePrefUI::GetSampleInterval() const {
  int time_value = wcstol(sample_interval_edit_->GetText(), NULL, 10);
  TimeScale scale = static_cast<TimeScale>(sample_interval_combo_->GetCurSel());
  switch (scale) {
  case SECOND:
    return TimeDelta::FromSeconds(time_value);
  case MINUTE:
    return TimeDelta::FromMinutes(time_value);
  case HOUR:
    return TimeDelta::FromHours(time_value);
  case DAY:
    return TimeDelta::FromDays(time_value);
  }
  return TimeDelta::Max();
}

void SampleModePrefUI::SetSampleInterval(const TimeDelta& delta) {
  TString button_text;
  if (delta.InHours() > 0) {
    button_text = StringUtility::IntToString(delta.InHours());
    button_text += kScaleStr[HOUR];
  } else if (delta.InMinutes() > 0) {
    button_text = StringUtility::IntToString(delta.InMinutes());
    button_text += kScaleStr[MINUTE];
  } else if (delta.InSeconds() > 0) {
    button_text = StringUtility::IntToString( static_cast<int>(delta.InSeconds()));
    button_text += kScaleStr[SECOND];
  }
  sample_interval_button_->SetText(button_text.c_str());
}

void SampleModePrefUI::SyncButtonToEdit() {
  DuiLib::CStdString  text = sample_interval_button_->GetText();
  std::wregex regex(_T("(\\d+)(\\w)"));
  std::wcmatch  match_results;
  if (std::regex_match(text.GetData(), text.GetData() + text.GetLength(), match_results, regex)) {
    sample_interval_edit_->SetText(match_results.str(1).c_str());
    sample_interval_combo_->SelectItem(ScaleStrToIndex(match_results.str(2)));
  }
}

bool SampleModePrefUI::OnButtonClicked(void* param) {
  DuiLib::TNotifyUI* notify = static_cast<DuiLib::TNotifyUI*>(param);
  if (notify->sType == _T("click")) {
    sample_interval_button_->SetVisible(false);
    sample_interval_ui_->SetVisible(true);
    sample_interval_edit_->SetFocus();
    return true;
  }
  return false;
}

bool SampleModePrefUI::OnSampleIntervalEditOKClicked(void* param) {
  DuiLib::TNotifyUI* notify = static_cast<DuiLib::TNotifyUI*>(param);
  if (notify->sType == _T("click")) {
    SyncEditToButton();
    sample_interval_button_->SetVisible(true);
    sample_interval_ui_->SetVisible(false);
    return true;
  }
  return false;
}

void SampleModePrefUI::SyncEditToButton()
{
  TString text = sample_interval_edit_->GetText();
  text += kScaleStr[sample_interval_combo_->GetCurSel()];
  sample_interval_button_->SetText(text.c_str());
}
