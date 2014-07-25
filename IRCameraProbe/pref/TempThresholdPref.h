#pragma once
#include "rapidjson/document.h"

namespace camera {
class TempThresholdLinker;
class TempThresholdPref {
public:
  //get the max stable temperature
  int GetStableMaxTemp() const {
    return stable_max_temp_;
  }
  //get the max delta between the average temperature and the max temperature
  int GetStableDeltaTemp() const {
    return stable_delta_temp_;
  }
 
  //get the max delta between stable temperature and the max temperature
  int GetUnstableMaxDeltaTemp() const {
    return unstable_max_delta_temp_;
  }
  //get the max unstable temperature
  int GetUnstableMaxTemp() const {
    return unstable_max_temp_;
  }

  void  SerializeFromJson(rapidjson::Document& document);
  void  SerializeToJson(rapidjson::Document& document);
 
private:
  friend class TempThresholdLinker;
  int stable_max_temp_;
  int stable_delta_temp_;

  int unstable_max_delta_temp_;
  int unstable_max_temp_;
};
}