#pragma once
#include <windows.h>

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

class Time {
public:
  static Time GetCurrentTime();

  Date  GetDateFromTime() const;
private:
  SYSTEMTIME time_;
};
}