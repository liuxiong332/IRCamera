#pragma once

class SampleModePrefUI;

namespace camera {
class SampleModeLinker {
public:
  SampleModeLinker(SampleModePrefUI* ui);

  void SampleModeChanged();
private:
  SampleModePrefUI* sample_mode_ui_;
};
}