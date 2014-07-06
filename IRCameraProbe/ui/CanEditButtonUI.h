#pragma once
#include "IRCameraBasic.h"
#include "ButtonEditUITransform.h"
#include "UIlib.h"

class CanEditButtonUI: public DuiLib::CContainerUI {
public:
  CanEditButtonUI(DuiLib::CContainerUI* ui);

  TString GetButtonText() const;
private:
  virtual LPCTSTR GetClass() const {
    return _T("CanEditButtonUI");
  }

  virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;

  DuiLib::CButtonUI*  button_;
  ButtonEditUITransform trans_;
};