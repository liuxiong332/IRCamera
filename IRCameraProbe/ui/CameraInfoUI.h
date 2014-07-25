#pragma once
#include "ButtonEditUITransform.h"
#include <wtypes.h>
#include "IRCameraBasic.h"

namespace DuiLib {
class CContainerUI;
class CButtonUI;
class CRichEditUI;
class COptionUI;
}
 
class CameraInfoUI {
public:
  void Init(DuiLib::CContainerUI* container_ui);

  TString GetIPAddr() const;
  TString GetName() const;

  void SetIPAddr(const TString& str);
  void SetName(const TString& str);

  bool  IsChecked() const;
  void  EnableCheckbox(bool is_enable);

  DuiLib::CContainerUI* GetUnderlyingUI() {
    return container_ui_;
  }
private:
  ButtonEditUITransform name_ui_transform_;
  ButtonEditUITransform ipaddr_ui_transform_;

  DuiLib::CContainerUI* container_ui_;

  DuiLib::COptionUI*  checkbox_;
  DuiLib::CButtonUI*  name_button_;
  DuiLib::CButtonUI*  ipaddr_button_;
  DuiLib::CRichEditUI*  name_edit_;
  DuiLib::CRichEditUI*  ipaddr_edit_;
};
 