#include "SampleModePref.h"
#include "SampleModePrefObserver.h"
#include <assert.h>
#include <tchar.h>

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
 

const static char kSampleModePrefStr[] = "sample_mode";
const static char kSampleMode[] = "mode";
const static char kSampleDelta[] = "delta";

void  SampleModePref::SerializeFromJson(rapidjson::Document& document) {
  if (document.HasMember(kSampleModePrefStr)) {
    rapidjson::Value& pref = document[kSampleModePrefStr];
    if (pref.HasMember(kSampleMode) && pref.HasMember(kSampleDelta)) {
      mode_ = static_cast<SampleMode>(pref[kSampleMode].GetInt());
      time_delta_ = TimeDelta::FromInternalValue(pref[kSampleDelta].GetInt64());
    }
  }
}

void  SampleModePref::SerializeToJson(rapidjson::Document& document) {
  typedef rapidjson::GenericStringRef<TCHAR>  UTF16StringRef;
  if (document.HasMember(kSampleModePrefStr))
    document.RemoveMember(kSampleModePrefStr);
  rapidjson::Value pref(rapidjson::kObjectType);
  pref.AddMember(rapidjson::StringRef(kSampleMode), rapidjson::Value(mode_).Move(), document.GetAllocator());
  pref.AddMember(rapidjson::StringRef(kSampleDelta), rapidjson::Value(time_delta_.ToInternalValue()).Move(), document.GetAllocator());
  document.AddMember(rapidjson::StringRef(kSampleModePrefStr), pref, document.GetAllocator());
}
}