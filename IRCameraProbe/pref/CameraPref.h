#pragma once
#include "rapidjson/document.h"
#include "common/PreferenceSerializer.h"
#include "CameraInfoPref.h"
#include "SampleModePref.h"
#include "StableSampleTimePref.h"
#include "TempThresholdPref.h"

namespace camera {
class CameraPref {
public:
  CameraPref();
  ~CameraPref();

  static CameraPref* GetInstance();

  CameraInfoPref*  GetCameraInfoPref();
  SampleModePref*  GetSampleMode();
  StableSampleTimePref*  GetStableSampleTime();
  TempThresholdPref* GetTempThreshold();
private:

  void  SerializeFromJson(rapidjson::Document& document);
  void  SerializeToJson(rapidjson::Document& document);

  CameraInfoPref    camera_info_;
  SampleModePref        sample_mode_;
  StableSampleTimePref sample_time_;
  TempThresholdPref   time_threshold_;

  PreferenceSerializer  serializer_;
};
}