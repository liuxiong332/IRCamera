#pragma once

namespace DuiLib {
class CContainerUI;
}
class CanEditButtonUI;

class StableSampleTimePrefUI {
public:
  StableSampleTimePrefUI(DuiLib::CContainerUI* container_ui);

  int GetHour();
  void SetHour(int hour);
private:
  CanEditButtonUI*  time_ui_;
};