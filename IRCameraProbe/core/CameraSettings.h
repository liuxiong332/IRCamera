#pragma once
#include <vector>
#include "IRCameraBasic.h"

class CameraSettings
{
public:
  CameraSettings();

  std::vector<TString>& GetIPList() {
    return ip_list_;
  }

  //stable temperature is that the temperature distribution is uniform
  int  GetStableMaxTemp() const {
    return stable_max_temp_;
  }
  void SetStableMaxTemp(int temp) {
    stable_max_temp_ = temp;
  }

  //set and get the delta of temperature of stable and unstable average
  int  GetUnstableAverageMaxDeltaTemp() const {
    return unstable_average_max_delta_temp_;
  }
  void SetUnstableAverageMaxDeltaTemp(int temp) {
    unstable_average_max_delta_temp_ = temp;
  }

  //get and set the delta of the unstable average temperature and the unstable max temperature
  int  GetUnstableMaxDeltaTemp() const {
    return unstable_max_delta_temp_;
  }
  void SetUnstableMaxDeltaTemp(int temp) {
    unstable_max_delta_temp_ = temp;
  }

  int  GetCanMeasureMaxTemp() const {
    return can_measure_max_temp_;
  }
  void SetCanMeasureMaxTemp(int temp) {
    can_measure_max_temp_ = temp;
  }

  int  GetCanMeasureMinTemp() const {
    return can_measure_min_temp_;
  }
  void SetCanMeasureMinTemp(int temp) {
    can_measure_min_temp_ = temp;
  }
private:
  std::vector<TString>  ip_list_;
  int  stable_max_temp_;
  int  unstable_average_max_delta_temp_;
  int  unstable_max_delta_temp_;
  int  can_measure_min_temp_;
  int  can_measure_max_temp_;
};

