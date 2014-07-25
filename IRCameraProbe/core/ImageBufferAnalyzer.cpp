#include "ImageBufferAnalyzer.h"
#include "CameraImageBuffer.h"
#include "pref/CameraPref.h"
#include "common/TempTransform.h"
#include <algorithm>

namespace camera {
ImageBufferAnalyzer::ImageBufferAnalyzer(CameraImageBuffer* buffer) {
  buffer_ = buffer;
  float* img_buf = buffer->GetBuffer();
  aver_temp_ = 0;
  max_temp_ = std::numeric_limits<float>::min();
  min_temp_ = std::numeric_limits<float>::max();

  int width = buffer->GetWidth(), height = buffer->GetHeight();
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      float cur_temp = TransformKelvinToCelsius(img_buf[i*width + j]);
      aver_temp_ += cur_temp;
      max_temp_ = std::max(max_temp_, cur_temp);
      min_temp_ = std::min(min_temp_, cur_temp);
    }
  }
  aver_temp_ /= (width*height);
}
 
TempJudgeResult UnstableTempJudger::JudgeImageBuffer(CameraImageBuffer* buffer, float* exception_temp) {
  ImageBufferAnalyzer analyzer(buffer);
  TempThresholdPref* pref = CameraPref::GetInstance()->GetTempThreshold();
  if (analyzer.GetMaxTemp() > pref->GetUnstableMaxTemp()) {   //the max temperature cannot over the threshold
    *exception_temp = analyzer.GetMaxTemp();
    return UNSTABLE_TEMP_EXCEPTION;
  } else if (analyzer.GetMaxTemp() - average_temp_ > pref->GetUnstableMaxDeltaTemp()) {
    *exception_temp = analyzer.GetMaxTemp() - average_temp_;
    return UNSTABLE_DELTA_EXCEPTION;
  }
  return TEMP_NORMAL;
}
}