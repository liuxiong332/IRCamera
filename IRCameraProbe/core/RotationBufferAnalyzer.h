#pragma once
#include "common/Time.h"
class CameraImageBuffer;

namespace camera {
//when the camera rotate around, the RotationBufferAnalyzer analyze all of the CameraImageBuffer
class RotationBufferAnalyzer {
public:
  //the param time is the time of get the image buffer
  RotationBufferAnalyzer(const Time& time) : time_(time), buffer_len_(0) {}

  void AddImageBuffer(CameraImageBuffer* buffer);

  const Time& GetTime() const { return time_;  }
  float GetAverageTemp() const { return aver_temp_; }
  float GetMaxAverageTemp() const { return max_temp_; }
  float GetMinAverageTemp() const { return min_temp_; }
private:
  Time  time_;

  int buffer_len_;
  float aver_temp_;
  float max_temp_;
  float min_temp_;
};

class DateBufferAnalyzer {
public:
  DateBufferAnalyzer(const Date& date) : date_(date), delta_temp_(std::numeric_limits<float>::max()), stable_temp_(0.0f) {}

  void AddRotationAnalyzer(const RotationBufferAnalyzer* analyzer);

  float GetStableTemp() const { return stable_temp_; }
private:
  Date date_;
  float delta_temp_;
  float stable_temp_;
};
}