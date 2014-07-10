#pragma once
#include "common/Time.h"
#include "CameraRotator.h"
#include "TempJudgeResult.h"
class CameraImageBuffer;

namespace camera {
//when the camera rotate around, the RotationBufferAnalyzer analyze all of the CameraImageBuffer
class RotationBufferAnalyzer {
public:
  //the param time is the time of get the image buffer
  RotationBufferAnalyzer() {}

  void AddImageBuffer(CameraRotationPos pos, CameraImageBuffer* buffer);

  float GetAverageTemp() const { return aver_temp_; }

  float GetMaxTemp(CameraRotationPos* pos) const { 
    if (pos) *pos = max_rotation_pos_;
    return max_temp_; 
  }

  float GetMinTemp() const { return min_temp_; }

  TempJudgeResult JudgeImageBuffer(CameraRotationPos* pos, float* exception_temp);
private:
  int buffer_len_;
  float aver_temp_;
  
  CameraRotationPos max_rotation_pos_ ;
  float max_temp_;
  float min_temp_;
};
 
}