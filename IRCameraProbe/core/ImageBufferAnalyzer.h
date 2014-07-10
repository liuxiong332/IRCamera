#pragma once
#include "CameraRotator.h"
#include "TempJudgeResult.h"

class CameraImageBuffer;
namespace camera {
class ImageBufferAnalyzer {
public:
  ImageBufferAnalyzer(CameraImageBuffer* buffer);

  float GetAverageTemp() const { return aver_temp_; }
  float GetMaxTemp() const { return max_temp_; }
  float GetMinTemp() const { return min_temp_; }
private:
  CameraImageBuffer*  buffer_;
  float aver_temp_;
  float max_temp_;
  float min_temp_;
};

class ImageBufferJudger {
public:
  virtual TempJudgeResult JudgeImageBuffer(CameraImageBuffer* buffer, float* exception_temp) = 0;
  virtual ~ImageBufferJudger() {}
};

class UnstableTempJudger : public ImageBufferJudger {
public:
  UnstableTempJudger(float aver_temp) : average_temp_(aver_temp) {}
  virtual TempJudgeResult JudgeImageBuffer(CameraImageBuffer* buffer, float*) override;
private:
  float average_temp_;
};

}