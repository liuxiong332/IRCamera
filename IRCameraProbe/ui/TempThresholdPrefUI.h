#pragma once
#include "ButtonEditUITransform.h"

namespace DuiLib {
class CContainerUI;
}
class CanEditButtonUI;

class TempThresholdPrefUI {
public:
  TempThresholdPrefUI(DuiLib::CContainerUI* container_ui);

  //get the max stable temperature
  int GetStableMaxTemp() const;

  int GetStableDeltaTemp() const;
  //get the max delta between unstable average temperature and the max temperature
  int GetUnstableMaxDeltaTemp() const;
  //get the max unstable temperature
  int GetUnstableMaxTemp() const;
private:
  DuiLib::CContainerUI* temp_pref_layout_;
 
  CanEditButtonUI*  stable_max_ui_;
  CanEditButtonUI*  stable_delta_ui_;

  CanEditButtonUI* unstable_max_delta_ui_;
  CanEditButtonUI* unstable_max_ui_;
};