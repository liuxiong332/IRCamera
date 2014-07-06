#pragma once
#include "UIlib.h"
#include "CanEditButtonUI.h"

class CommonUIOperator {
public:
  static DuiLib::CControlUI* FindSubControlByName(DuiLib::CContainerUI* parent_ui, LPCTSTR name) {
    return parent_ui->GetManager()->FindSubControlByName(parent_ui, name);
  }

  static DuiLib::CContainerUI* FindSubContainerByName(DuiLib::CContainerUI* parent_ui, LPCTSTR name) {
    return static_cast<DuiLib::CContainerUI*>(FindSubControlByName(parent_ui, name));
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

  static DuiLib::COptionUI* FindSubOptionByName(DuiLib::CContainerUI* parent_ui, LPCTSTR name) {
    return static_cast<DuiLib::COptionUI*>(FindSubControlByName(parent_ui, name));
  }

  static DuiLib::CComboUI* FindSubComboByName(DuiLib::CContainerUI* parent_ui, LPCTSTR name) {
    return static_cast<DuiLib::CComboUI*>(FindSubControlByName(parent_ui, name));
  }

  static CanEditButtonUI* FindSubCanEditButtonByName(DuiLib::CContainerUI* parent_ui, LPCTSTR name) {
    return static_cast<CanEditButtonUI*>(FindSubControlByName(parent_ui, name));
  }
};