#pragma once
#include <limits>
// TimeDelta ------------------------------------------------------------------

typedef long long int64;
class TimeDelta {
public:
  
  TimeDelta() : delta_(0) {
  }

  // Converts units of time to TimeDeltas.
  static TimeDelta FromDays(int days);
  static TimeDelta FromHours(int hours);
  static TimeDelta FromMinutes(int minutes);
  static TimeDelta FromSeconds(int64 secs);
  static TimeDelta FromMilliseconds(int64 ms);
  static TimeDelta FromSecondsD(double secs);
  static TimeDelta FromMillisecondsD(double ms);
  static TimeDelta FromMicroseconds(int64 us);
#if defined(OS_WIN)
  static TimeDelta FromQPCValue(LONGLONG qpc_value);
#endif

  // Converts an integer value representing TimeDelta to a class. This is used
  // when deserializing a |TimeDelta| structure, using a value known to be
  // compatible. It is not provided as a constructor because the integer type
  // may be unclear from the perspective of a caller.
  static TimeDelta FromInternalValue(int64 delta) {
    return TimeDelta(delta);
  }

  // Returns the maximum time delta, which should be greater than any reasonable
  // time delta we might compare it to. Adding or subtracting the maximum time
  // delta to a time or another time delta has an undefined result.
  static TimeDelta Max();

  // Returns the internal numeric value of the TimeDelta object. Please don't
  // use this and do arithmetic on it, as it is more error prone than using the
  // provided operators.
  // For serializing, use FromInternalValue to reconstitute.
  int64 ToInternalValue() const {
    return delta_;
  }

  // Returns true if the time delta is the maximum time delta.
  bool is_max() const {
    return delta_ == std::numeric_limits<int64>::max();
  }

  // Returns the time delta in some unit. The F versions return a floating
  // point value, the "regular" versions return a rounded-down value.
  //
  // InMillisecondsRoundedUp() instead returns an integer that is rounded up
  // to the next full millisecond.
  int InDays() const;
  int InHours() const;
  int InMinutes() const;
  double InSecondsF() const;
  int64 InSeconds() const;
  double InMillisecondsF() const;
  int64 InMilliseconds() const;
  int64 InMillisecondsRoundedUp() const;
  int64 InMicroseconds() const;

  TimeDelta& operator=(TimeDelta other) {
    delta_ = other.delta_;
    return *this;
  }

  // Computations with other deltas.
  TimeDelta operator+(TimeDelta other) const {
    return TimeDelta(delta_ + other.delta_);
  }
  TimeDelta operator-(TimeDelta other) const {
    return TimeDelta(delta_ - other.delta_);
  }

  TimeDelta& operator+=(TimeDelta other) {
    delta_ += other.delta_;
    return *this;
  }
  TimeDelta& operator-=(TimeDelta other) {
    delta_ -= other.delta_;
    return *this;
  }
  TimeDelta operator-() const {
    return TimeDelta(-delta_);
  }

  // Computations with ints, note that we only allow multiplicative operations
  // with ints, and additive operations with other deltas.
  TimeDelta operator*(int64 a) const {
    return TimeDelta(delta_ * a);
  }
  TimeDelta operator/(int64 a) const {
    return TimeDelta(delta_ / a);
  }
  TimeDelta& operator*=(int64 a) {
    delta_ *= a;
    return *this;
  }
  TimeDelta& operator/=(int64 a) {
    delta_ /= a;
    return *this;
  }
  int64 operator/(TimeDelta a) const {
    return delta_ / a.delta_;
  }

  // Comparison operators.
  bool operator==(TimeDelta other) const {
    return delta_ == other.delta_;
  }
  bool operator!=(TimeDelta other) const {
    return delta_ != other.delta_;
  }
  bool operator<(TimeDelta other) const {
    return delta_ < other.delta_;
  }
  bool operator<=(TimeDelta other) const {
    return delta_ <= other.delta_;
  }
  bool operator>(TimeDelta other) const {
    return delta_ > other.delta_;
  }
  bool operator>=(TimeDelta other) const {
    return delta_ >= other.delta_;
  }

private:
  friend TimeDelta operator*(int64 a, TimeDelta td);

  // Constructs a delta given the duration in microseconds. This is private
  // to avoid confusion by callers with an integer constructor. Use
  // FromSeconds, FromMilliseconds, etc. instead.
  explicit TimeDelta(int64 delta_us) : delta_(delta_us) {
  }

  // Delta in microseconds.
  int64 delta_;

  static const int64 kMillisecondsPerSecond = 1000;
  static const int64 kMicrosecondsPerMillisecond = 1000;
  static const int64 kMicrosecondsPerSecond = kMicrosecondsPerMillisecond *
    kMillisecondsPerSecond;
  static const int64 kMicrosecondsPerMinute = kMicrosecondsPerSecond * 60;
  static const int64 kMicrosecondsPerHour = kMicrosecondsPerMinute * 60;
  static const int64 kMicrosecondsPerDay = kMicrosecondsPerHour * 24;
  static const int64 kMicrosecondsPerWeek = kMicrosecondsPerDay * 7;
  static const int64 kNanosecondsPerMicrosecond = 1000;
  static const int64 kNanosecondsPerSecond = kNanosecondsPerMicrosecond *
    kMicrosecondsPerSecond;

};

// static
inline TimeDelta TimeDelta::FromDays(int days) {
  // Preserve max to prevent overflow.
  if (days == std::numeric_limits<int>::max())
    return Max();
  return TimeDelta(days * kMicrosecondsPerDay);
}

// static
inline TimeDelta TimeDelta::FromHours(int hours) {
  // Preserve max to prevent overflow.
  if (hours == std::numeric_limits<int>::max())
    return Max();
  return TimeDelta(hours * kMicrosecondsPerHour);
}

// static
inline TimeDelta TimeDelta::FromMinutes(int minutes) {
  // Preserve max to prevent overflow.
  if (minutes == std::numeric_limits<int>::max())
    return Max();
  return TimeDelta(minutes * kMicrosecondsPerMinute);
}

// static
inline TimeDelta TimeDelta::FromSeconds(int64 secs) {
  // Preserve max to prevent overflow.
  if (secs == std::numeric_limits<int64>::max())
    return Max();
  return TimeDelta(secs * kMicrosecondsPerSecond);
}

// static
inline TimeDelta TimeDelta::FromMilliseconds(int64 ms) {
  // Preserve max to prevent overflow.
  if (ms == std::numeric_limits<int64>::max())
    return Max();
  return TimeDelta(ms * kMicrosecondsPerMillisecond);
}

// static
inline TimeDelta TimeDelta::FromSecondsD(double secs) {
  // Preserve max to prevent overflow.
  if (secs == std::numeric_limits<double>::infinity())
    return Max();
  return TimeDelta(static_cast<int64>(secs * kMicrosecondsPerSecond));
}

// static
inline TimeDelta TimeDelta::FromMillisecondsD(double ms) {
  // Preserve max to prevent overflow.
  if (ms == std::numeric_limits<double>::infinity())
    return Max();
  return TimeDelta(static_cast<int64>(ms * kMicrosecondsPerMillisecond));
}

// static
inline TimeDelta TimeDelta::FromMicroseconds(int64 us) {
  // Preserve max to prevent overflow.
  if (us == std::numeric_limits<int64>::max())
    return Max();
  return TimeDelta(us);
}