#include "TimeDelta.h"

// static
TimeDelta TimeDelta::Max() {
  return TimeDelta(std::numeric_limits<int64>::max());
}

int TimeDelta::InDays() const {
  if (is_max()) {
    // Preserve max to prevent overflow.
    return std::numeric_limits<int>::max();
  }
  return static_cast<int>(delta_ / kMicrosecondsPerDay);
}

int TimeDelta::InHours() const {
  if (is_max()) {
    // Preserve max to prevent overflow.
    return std::numeric_limits<int>::max();
  }
  return static_cast<int>(delta_ / kMicrosecondsPerHour);
}

int TimeDelta::InMinutes() const {
  if (is_max()) {
    // Preserve max to prevent overflow.
    return std::numeric_limits<int>::max();
  }
  return static_cast<int>(delta_ / kMicrosecondsPerMinute);
}

double TimeDelta::InSecondsF() const {
  if (is_max()) {
    // Preserve max to prevent overflow.
    return std::numeric_limits<double>::infinity();
  }
  return static_cast<double>(delta_) / kMicrosecondsPerSecond;
}

int64 TimeDelta::InSeconds() const {
  if (is_max()) {
    // Preserve max to prevent overflow.
    return std::numeric_limits<int64>::max();
  }
  return delta_ / kMicrosecondsPerSecond;
}

double TimeDelta::InMillisecondsF() const {
  if (is_max()) {
    // Preserve max to prevent overflow.
    return std::numeric_limits<double>::infinity();
  }
  return static_cast<double>(delta_) / kMicrosecondsPerMillisecond;
}

int64 TimeDelta::InMilliseconds() const {
  if (is_max()) {
    // Preserve max to prevent overflow.
    return std::numeric_limits<int64>::max();
  }
  return delta_ / kMicrosecondsPerMillisecond;
}

int64 TimeDelta::InMillisecondsRoundedUp() const {
  if (is_max()) {
    // Preserve max to prevent overflow.
    return std::numeric_limits<int64>::max();
  }
  return (delta_ + kMicrosecondsPerMillisecond - 1) /
    kMicrosecondsPerMillisecond;
}

int64 TimeDelta::InMicroseconds() const {
  if (is_max()) {
    // Preserve max to prevent overflow.
    return std::numeric_limits<int64>::max();
  }
  return delta_;
}
