#include "CameraPref.h"
#include <tchar.h>

namespace camera {
 
CameraPref* CameraPref::GetInstance() {
  static CameraPref kPref;
  return &kPref;
}

CameraPref::CameraPref() {
  SerializeFromJson(serializer_.GetDocument());
}

CameraPref::~CameraPref() {
  SerializeToJson(serializer_.GetDocument());
}
CameraInfoPref*  CameraPref::GetCameraInfoPref() {
  return &camera_info_;
}

SampleModePref*  CameraPref::GetSampleMode() {
  return &sample_mode_;
}

StableSampleTimePref*  CameraPref::GetStableSampleTime() {
  return &sample_time_;
}

TempThresholdPref* CameraPref::GetTempThreshold() {
  return &time_threshold_;
}

const static char kStableSampleTimeStr[] = "stable_sample_time";

void  CameraPref::SerializeFromJson(rapidjson::Document& document) {
  if (document.IsNull() || !document.IsObject())
    return;
  camera_info_.SerializeFromJson(document);
  sample_mode_.SerializeFromJson(document);

  if (document.HasMember(kStableSampleTimeStr)) {
    sample_time_.SetSampleHour(document[kStableSampleTimeStr].GetInt());
  }
  time_threshold_.SerializeFromJson(document);
}
void  CameraPref::SerializeToJson(rapidjson::Document& document) {
  if (!document.IsObject())
    document.SetObject();
  camera_info_.SerializeToJson(document);
  sample_mode_.SerializeToJson(document);

  document.RemoveMember(kStableSampleTimeStr);
  document.AddMember(rapidjson::StringRef(kStableSampleTimeStr), 
    rapidjson::Value(sample_time_.GetSampleHour()).Move(), document.GetAllocator());
  time_threshold_.SerializeToJson(document);
}
}