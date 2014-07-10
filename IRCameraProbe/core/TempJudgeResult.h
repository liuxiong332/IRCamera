#pragma once

namespace camera {
enum TempJudgeResult {
  TEMP_NORMAL,
  STABLE_TEMP_EXCEPTION, /*stable temperature over the threshold*/
  STABLE_DELTA_EXCEPTION,
  UNSTABLE_TEMP_EXCEPTION,
  UNSTABLE_DELTA_EXCEPTION
};
}