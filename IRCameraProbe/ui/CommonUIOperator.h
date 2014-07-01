#pragma once
#include "UIlib.h"

class CommonUIOperator {
public:
  static DuiLib::CControlUI* FindSubControlByName(DuiLib::CContainerUI* parent_ui, LPCTSTR name) {
    return parent_ui->GetManager()->FindSubControlByName(parent_ui, name);
  }

  static DuiLib::CLabelUI* FindSubLabelByName(DuiLib::CContainerUI* parent_ui, LPCTSTR name) {
    return static_cast<DuiLib::CLabelUI*>(FindSubControlByName(parent_ui, name));
  }

  static DuiLib::CButtonUI*  FindSubButtonByName(DuiLib::CContainerUI* parent_ui, LPCTSTR name) {
    return static_cast<DuiLib::CButtonUI*>(FindSubControlByName(parent_ui, name));
  }

  static DuiLib::CRichEditUI*  FindSubRichEditByName(DuiLib::CContainerUI* parent_ui, LPCTSTR name) {
    return static_cast<DuiLib::CRichEditUI*>(FindSubControlByName(parent_ui, name));
  }
};