#include "CanEditButtonUI.h"
#include "CommonUIOperator.h"

CanEditButtonUI::CanEditButtonUI(DuiLib::CContainerUI* ui) {
  button_ = CommonUIOperator::FindSubButtonByName(ui, _T("canedit_btn"));
  DuiLib::CRichEditUI* edit = CommonUIOperator::FindSubRichEditByName(ui, _T("canedit_edit"));
  trans_.Init(button_, edit);
  Add(ui);
}

TString CanEditButtonUI::GetButtonText() const {
  return TString(button_->GetText().GetData());
}

void  CanEditButtonUI::SetButtonText(const TString& str) {
  button_->SetText(str.c_str());
}

void CanEditButtonUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) {
  if (_tcscmp(pstrName, _T("buttontext")) == 0) {
    button_->SetText(pstrValue);
  } 
  else DuiLib::CContainerUI::SetAttribute(pstrName, pstrValue);
}