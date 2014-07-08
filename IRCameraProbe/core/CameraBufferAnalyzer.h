#pragma once
#include "CameraRotator.h"

class CameraImageBuffer;
namespace camera {
class CameraBufferAnalyzer {
public:
  CameraBufferAnalyzer(CameraImageBuffer* buffer);

  float GetAverageTemp() const { return aver_temp_; }
  float GetMaxAverageTemp() const { return max_temp_; }
  float GetMinAverageTemp() const { return min_temp_; }
private:
  CameraImageBuffer*  buffer_;
  float aver_temp_;
  float max_temp_;
  float min_temp_;
};
}