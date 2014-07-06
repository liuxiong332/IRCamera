#pragma once

namespace DuiLib {
class CControlUI;
}
class CanEditButtonUI;
class CanEditButtonUICreator {
public:
  virtual CanEditButtonUI* CreateCanEditButtonUI() = 0;
  virtual ~CanEditButtonUICreator() {}
};