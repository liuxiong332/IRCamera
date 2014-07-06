#pragma once

class TempThresholdPrefUI;
namespace camera {
class TempThresholdLinker {
public:
  TempThresholdLinker(TempThresholdPrefUI* ui);

  void TempThresholdChanged();
private:
  TempThresholdPrefUI*  pref_ui_;
};
}