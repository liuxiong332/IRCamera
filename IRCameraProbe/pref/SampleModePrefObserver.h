#pragma once
#include "SampleMode.h"
#include "common/TimeDelta.h"

namespace camera {
class SampleModePrefObserver {
public:
  virtual void OnSampleModeChanged(SampleMode) = 0;
  virtual void OnTimeDeltaChanged(const TimeDelta&) = 0;
  virtual ~SampleModePrefObserver() {}
};
}