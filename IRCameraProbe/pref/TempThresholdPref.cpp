#include "TempThresholdPref.h"
#include <tchar.h>

const static char kStableMaxTempStr[] = "stable_max_temp";
const static char kStableDeltaTemp[] = "stable_delta_temp";
const static char kUnstableMaxDeltaTemp[] = "unstable_max_delta_temp";
const static char kUnstableMaxTemp[] = "unstable_max_temp";

namespace camera {
void  TempThresholdPref::SerializeFromJson(rapidjson::Document& document) {
  if (document.HasMember(kStableMaxTempStr)) {
    stable_max_temp_ = document[kStableMaxTempStr].GetInt();
  }
  if (document.HasMember(kStableDeltaTemp)) {
    stable_delta_temp_ = document[kStableDeltaTemp].GetInt();
  }
  if (document.HasMember(kUnstableMaxDeltaTemp)) {
    unstable_max_delta_temp_ = document[kUnstableMaxDeltaTemp].GetInt();
  }
  if (document.HasMember(kUnstableMaxTemp)) {
    unstable_max_temp_ = document[kUnstableMaxTemp].GetInt();
  }
}

void SetMember(rapidjson::Document& document, const char* key, int value) {
  if (!document.HasMember(key)) {
    document.AddMember(rapidjson::StringRef(key), rapidjson::Value(0).Move(), document.GetAllocator());
  }
  document[key].SetInt(value);
}
  
void  TempThresholdPref::SerializeToJson(rapidjson::Document& document) {
  SetMember(document, kStableMaxTempStr, stable_max_temp_);
  SetMember(document, kStableDeltaTemp, stable_delta_temp_);
  SetMember(document, kUnstableMaxDeltaTemp, unstable_max_delta_temp_);
  SetMember(document, kUnstableMaxTemp, unstable_max_temp_);
}
}