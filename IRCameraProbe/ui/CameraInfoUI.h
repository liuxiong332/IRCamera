#pragma once
#include "ButtonEditUITransform.h"
#include <wtypes.h>

namespace DuiLib {
class CContainerUI;
class CButtonUI;
class CRichEditUI;
}
 
class CameraInfoUI {
public:
  void Init(DuiLib::CContainerUI* container_ui);

  LPCTSTR GetIPAddr() const;
  LPCTSTR GetName() const;
private:
  ButtonEditUITransform name_ui_transform_;
  ButtonEditUITransform ipaddr_ui_transform_;

  DuiLib::CButtonUI*  name_button_;
  DuiLib::CButtonUI*  ipaddr_button_;
  DuiLib::CRichEditUI*  name_edit_;
  DuiLib::CRichEditUI*  ipaddr_edit_;
};
 