#include "RotationBufferAnalyzer.h"
#include "ImageBufferAnalyzer.h"
#include "pref/TempThresholdPref.h"
#include <algorithm>

namespace camera {
void RotationBufferAnalyzer::AddImageBuffer(CameraRotationPos pos, CameraImageBuffer* buffer) {
  ImageBufferAnalyzer buffer_analyzer(buffer);
  if (buffer_len_ == 0) {   //the first camera image buffer
    aver_temp_ = buffer_analyzer.GetAverageTemp();
    max_temp_ = buffer_analyzer.GetMaxTemp();
    max_rotation_pos_ = pos;
    min_temp_ = buffer_analyzer.GetMinTemp();
  } else {
    aver_temp_ = (aver_temp_*buffer_len_ + buffer_analyzer.GetAverageTemp()) / (buffer_len_ + 1);
    if (buffer_analyzer.GetMaxTemp() > max_temp_) {
      max_temp_ = buffer_analyzer.GetMaxTemp();
      max_rotation_pos_ = pos;
    }
    min_temp_ = std::min(min_temp_, buffer_analyzer.GetMinTemp());
  }
  ++buffer_len_;
}

TempJudgeResult RotationBufferAnalyzer::JudgeImageBuffer(CameraRotationPos* pos, float* exception_temp) {
  TempThresholdPref* pref = TempThresholdPref::GetInstance();
  if (GetMaxTemp(pos) > pref->GetStableMaxTemp()) {
    *exception_temp = GetMaxTemp(pos);
    return STABLE_TEMP_EXCEPTION;
  }
  else if (GetMaxTemp(pos) - GetAverageTemp() > pref->GetStableDeltaTemp()) {
    *exception_temp = GetMaxTemp(pos) - GetAverageTemp();
    return STABLE_DELTA_EXCEPTION;
  }
  return TEMP_NORMAL;
}

}