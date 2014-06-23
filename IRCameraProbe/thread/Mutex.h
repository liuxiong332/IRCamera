#pragma once
#include <windows.h>

namespace camera {

class Mutex {
public:
  Mutex();
  void Lock();
  void Unlock();
  ~Mutex();
private:
  HANDLE mutex_;
};

class ScopedMutex {
public:
  ScopedMutex(Mutex* mutex);
  ~ScopedMutex();
private:
  Mutex* mutex_;
};
}