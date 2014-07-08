#include "TempJudge.h"
#include "pref/TempThresholdPref.h"
#include "RotationBufferAnalyzer.h"

namespace camera {

TempJudge::JudgeResult TempJudge::JudgeUnstableTemp(RotationBufferAnalyzer* analyzer) {
  TempThresholdPref* pref = TempThresholdPref::GetInstance();
  if (analyzer->GetAverageTemp() > pref->GetUnstableAverTemp()) {
    return AVERAGE_TEMP_OVERFLOW;
  }
  else if (analyzer->GetAverageTemp() - average_temp_ > pref->GetUnstableAverDeltaTemp()) {
    return AVERAGE_TEMP_DELTA_OVERFLOW;
  }
  else if (analyzer->GetMaxTemp() > pref->GetUnstableMaxTemp()) {
    return MAX_TEMP_OVERFLOW;
  }
  else if (analyzer->GetMaxTemp() - analyzer->GetAverageTemp() > pref->GetUnstableMaxDeltaTemp()) {
    return MAX_TEMP_DELTA_OVERFLOW;
  }
  return TEMP_NORMAL;
}

TempJudge::JudgeResult TempJudge::JudgeStableTemp(float stable_temp) {
  TempThresholdPref* pref = TempThresholdPref::GetInstance();
  if (stable_temp > pref->GetStableMaxTemp()) {
    return AVERAGE_TEMP_OVERFLOW;
  }
  return TEMP_NORMAL;
}
}