#include "stdafx.h"
#include "Mutex.h"

namespace camera {

Mutex::Mutex() {
  InitializeCriticalSection(&cs_);
}

void  Mutex::Lock() {
  EnterCriticalSection(&cs_);
}

void Mutex::Unlock() {
  LeaveCriticalSection(&cs_);
}

Mutex::~Mutex() {
  DeleteCriticalSection(&cs_);
}

ScopedMutex::ScopedMutex(Mutex* mutex) {
  mutex_ = mutex;
  mutex_->Lock();
}

ScopedMutex::~ScopedMutex() {
  mutex_->Unlock();
}

}