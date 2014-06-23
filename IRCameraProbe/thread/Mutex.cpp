#include "stdafx.h"
#include "Mutex.h"

namespace camera {

Mutex::Mutex() {
  mutex_ = CreateMutex(NULL, FALSE, NULL);
}

void  Mutex::Lock() {
  ::WaitForSingleObject(mutex_, INFINITE);
}

void Mutex::Unlock() {
  ReleaseMutex(mutex_);
}

Mutex::~Mutex() {
  CloseHandle(mutex_);
}

ScopedMutex::ScopedMutex(Mutex* mutex) {
  mutex_ = mutex;
  mutex_->Lock();
}

ScopedMutex::~ScopedMutex() {
  mutex_->Unlock();
}

}