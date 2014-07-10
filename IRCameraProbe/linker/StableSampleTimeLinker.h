#pragma once

class StableSampleTimePrefUI;
namespace camera {
class StableSampleTimeLinker {
public:
  StableSampleTimeLinker(StableSampleTimePrefUI* ui);

  void StableSampleTimeChanged();
private:
  StableSampleTimePrefUI* pref_ui_;
};
}