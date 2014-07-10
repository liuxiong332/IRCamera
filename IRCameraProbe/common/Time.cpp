#include "time.h"

namespace camera {
Time Time::GetCurrentTime() {
  Time cur_time;
  ::GetLocalTime(&cur_time.time_);
  return cur_time;
}

Time::Time(short hour, short minute, short second) {
  time_.wHour = hour;
  time_.wMinute = minute;
  time_.wSecond = second;
}

Date  Time::GetDateFromTime() const {
  return Date(time_.wYear, time_.wMonth, time_.wDay);
}

int  Time::GetHour() const {
  return time_.wHour;
}

void Time::SetHour(int hour) {
  time_.wHour = hour;
}

long long Time::HourMinuteSecondToMilli() const {
  return (time_.wHour * TimeDelta::kMicrosecondsPerHour +
    time_.wMinute * TimeDelta::kMicrosecondsPerMinute +
    time_.wSecond * TimeDelta::kMicrosecondsPerSecond) / TimeDelta::kMicrosecondsPerMillisecond;
}


}