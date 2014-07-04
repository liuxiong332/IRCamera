#include "SampleModePref.h"
#include "SampleModePrefObserver.h"
#include <assert.h>

namespace camera {

SampleModePref::SampleModePref() :mode_(MANUAL_MODE), observer_(NULL) {
}

void SampleModePref::InitWithObserver(SampleModePrefObserver* observer) {
  assert(observer_ == NULL);
  observer_ = observer;
}

void SampleModePref::SetSampleMode(SampleMode mode) {
  if (mode_ != mode && observer_) {
    observer_->OnSampleModeChanged(mode);
  }
  mode_ = mode;
}
  
void SampleModePref::SetSampleInterval(const TimeDelta& delta) {
  if (delta != time_delta_ && observer_) {
    observer_->OnTimeDeltaChanged(delta);
  }
  time_delta_ = delta;
}

SampleModePref*  SampleModePref::GetInstance() {
  static SampleModePref kSamplePref;
  return &kSamplePref;
}

}