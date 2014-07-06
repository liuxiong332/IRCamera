#pragma once

namespace camera {
class TempThresholdLinker;
class TempThresholdPref {
public:
  //get the max stable temperature
  int GetStableMaxTemp() const {
    return stable_max_temp_;
  }
  //get the max delta between the stable and unstable average temperature
  int GetUnstableAverDeltaTemp() const {
    return unstable_aver_delta_temp_;
  }
  //get the max unstable average temperature
  int GetUnstableAverTemp() const {
    return unstable_aver_temp_;
  }
  //get the max delta between unstable average temperature and the max temperature
  int GetUnstableMaxDeltaTemp() const {
    return unstable_max_delta_temp_;
  }
  //get the max unstable temperature
  int GetUnstableMaxTemp() const {
    return unstable_max_temp_;
  }

  static TempThresholdPref*  GetInstance() {
    static TempThresholdPref pref;
    return &pref;
  }
private:
  friend class TempThresholdLinker;
  int stable_max_temp_;
  int unstable_aver_delta_temp_;
  int unstable_aver_temp_;
  int unstable_max_delta_temp_;
  int unstable_max_temp_;
};
}