#pragma once

//switch between every tab layout
class TabLayoutSwitcher {
public:
  virtual void SwitchToContentLayout() = 0;
  virtual void SwitchToPrefLayout() = 0;

  virtual ~TabLayoutSwitcher()  {}
};