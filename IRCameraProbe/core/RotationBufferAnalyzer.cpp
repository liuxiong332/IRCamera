#include "RotationBufferAnalyzer.h"
#include "CameraBufferAnalyzer.h"
#include <algorithm>

namespace camera {
void RotationBufferAnalyzer::AddImageBuffer(CameraImageBuffer* buffer) {
  CameraBufferAnalyzer buffer_analyzer(buffer);
  if (buffer_len_ == 0) {   //the first camera image buffer
    aver_temp_ = buffer_analyzer.GetAverageTemp();
    max_temp_ = buffer_analyzer.GetMaxAverageTemp();
    min_temp_ = buffer_analyzer.GetMinAverageTemp();
  } else {
    aver_temp_ = (aver_temp_*buffer_len_ + buffer_analyzer.GetAverageTemp()) / (buffer_len_ + 1);
    max_temp_ = std::max(max_temp_, buffer_analyzer.GetMaxAverageTemp());
    min_temp_ = std::min(min_temp_, buffer_analyzer.GetMinAverageTemp());
  }
  ++buffer_len_;
}

void DateBufferAnalyzer::AddRotationAnalyzer(const RotationBufferAnalyzer* analyzer) {
  //if the delta between the max temp and the min temp is less than the current delta,
  //then update the stable temperature
  if (analyzer->GetMaxAverageTemp() - analyzer->GetMinAverageTemp() < delta_temp_)
    stable_temp_ = analyzer->GetAverageTemp();
}
}