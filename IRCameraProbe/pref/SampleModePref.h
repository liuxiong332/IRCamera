#pragma once
#include "common/TimeDelta.h"
#include "SampleMode.h"

namespace camera {
class SampleModePrefObserver;
class SampleModePref {
public:
  SampleModePref();
  void InitWithObserver(SampleModePrefObserver* observer);

  SampleMode  GetSampleMode() const {
    return mode_;
  }

  TimeDelta GetSampleInterval() const {
    return time_delta_;
  }

  void SetSampleMode(SampleMode mode);
  void SetSampleInterval(const TimeDelta& delta);

  static SampleModePref*  GetInstance();
private:
  SampleModePrefObserver* observer_;
  SampleMode mode_;
  TimeDelta time_delta_;
};
}