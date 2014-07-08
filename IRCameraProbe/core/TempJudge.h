#pragma once

namespace camera {
class RotationBufferAnalyzer;
class TempJudge {
public:
  enum JudgeResult {
    TEMP_NORMAL,
    MAX_TEMP_OVERFLOW,
    MAX_TEMP_DELTA_OVERFLOW,
    AVERAGE_TEMP_OVERFLOW,
    AVERAGE_TEMP_DELTA_OVERFLOW
  };
  TempJudge(float average_temp) : average_temp_(average_temp) {}

  JudgeResult JudgeUnstableTemp(RotationBufferAnalyzer* analyzer);
private:
  float average_temp_;
};
}