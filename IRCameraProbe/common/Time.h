#pragma once
#include <windows.h>
#include "TimeDelta.h"

namespace camera {

class Date {
public:
  Date() : year_(0), month_(0), day_(0) {}
  Date(int year, int month, int day) : year_(year), month_(month), day_(day) {}
  int GetYear() const { return year_;  }
  int GetMonth() const { return month_;  }
  int GetDay() const { return day_;  }
private:
  int year_;
  int month_;
  int day_;
};
inline bool operator==(const Date& date1, const Date& date2) {
  return date1.GetDay() == date2.GetDay() &&
    date1.GetMonth() == date2.GetMonth() &&
    date1.GetYear() == date2.GetYear();
}

inline bool operator!=(const Date& date1, const Date& date2) {
  return !(date1 == date2);
}

class Time {
public:
  static Time GetCurrentTime();

  Time() {}
  Time(short hour, short minute, short second);

  Date  GetDateFromTime() const;

  int  GetHour() const;
  void SetHour(int hour);

  long long HourMinuteSecondToMilli() const;
private:
  friend TimeDelta operator-(const Time& time1, const Time& time2);
  SYSTEMTIME time_;
};

TimeDelta operator-(const Time& time1, const Time& time2);
}