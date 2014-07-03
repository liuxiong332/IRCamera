#include "ButtonEditUITransform.h"
#include "UIlib.h"

void ButtonEditUITransform::Init(DuiLib::CButtonUI* button, DuiLib::CControlUI* control_ui) {
  button_ui_ = button;
  control_ui_ = control_ui;
  button_ui_->SetVisible(true);
  button_ui_->OnNotify += DuiLib::MakeDelegate(this, &ButtonEditUITransform::OnButtonClicked);
  control_ui->SetVisible(false);
  control_ui->OnNotify += DuiLib::MakeDelegate(this, &ButtonEditUITransform::OnControlKillFocus);
}

bool ButtonEditUITransform::OnButtonClicked(void* param) {
  DuiLib::TNotifyUI* notify = static_cast<DuiLib::TNotifyUI*>(param);
  if (notify->sType == _T("click")) {
    button_ui_->SetVisible(false);
    control_ui_->SetText(button_ui_->GetText());
    control_ui_->SetVisible(true);  //make the edit control visible
    control_ui_->SetFocus();
    return true;
  }
  return false;
}
bool ButtonEditUITransform::OnControlKillFocus(void* param) {
  DuiLib::TNotifyUI* notify = static_cast<DuiLib::TNotifyUI*>(param);
  if (notify->sType == _T("killfocus")) {
    control_ui_->SetVisible(false);
    button_ui_->SetText(control_ui_->GetText());
    button_ui_->SetVisible(true);
    return true;
  }
  return false;
}