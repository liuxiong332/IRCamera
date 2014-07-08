#include "time.h"

namespace camera {
Time Time::GetCurrentTime() {
  Time cur_time;
  ::GetLocalTime(&cur_time.time_);
  return cur_time;
}

Date  Time::GetDateFromTime() const {
  return Date(time_.wYear, time_.wMonth, time_.wDay);
}
}