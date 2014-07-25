#pragma once
#include "StableSampleTimePrefObserver.h"
#include <stdlib.h>
#include "common/PreferenceSerializer.h"

namespace camera {
class StableSampleTimePrefObserver;
class StableSampleTimePref {
public:
  StableSampleTimePref() : observer_(NULL) {}
  void Init(StableSampleTimePrefObserver* observer) {
    observer_ = observer;
  }

  int GetSampleHour() const {
    return hour_;
  }

  void SetSampleHour(int hour) {
    hour_ = hour;
    if (observer_)
     observer_->OnSampleTimeChanged();
  }
 
private:
  StableSampleTimePrefObserver* observer_;
  int hour_;
};
}