#pragma once

namespace camera {
class StableSampleTimePrefObserver{
public:
  virtual void OnSampleTimeChanged() = 0;
  virtual ~StableSampleTimePrefObserver() {}
};
}