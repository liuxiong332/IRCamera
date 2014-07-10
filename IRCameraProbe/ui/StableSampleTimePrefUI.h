#pragma once

namespace DuiLib {
class CContainerUI;
}
class CanEditButtonUI;

class StableSampleTimePrefUI {
public:
  StableSampleTimePrefUI(DuiLib::CContainerUI* container_ui);

  int GetHour();
private:
  CanEditButtonUI*  time_ui_;
};