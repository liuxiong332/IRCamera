#pragma once
#include <windows.h>
#include <assert.h>

template<typename T>
class ThreadLocalStorage
{
public:
  ThreadLocalStorage();
  ~ThreadLocalStorage();

  void SetValue(const T* value);
  T* GetValue();
private:
  DWORD  tls_index;
};

template<typename T>
ThreadLocalStorage<T>::ThreadLocalStorage() {
  tls_index = TlsAlloc();
  assert(tls_index != TLS_OUT_OF_INDEXES);
}

template<typename T>
ThreadLocalStorage<T>::~ThreadLocalStorage() {
  TlsFree(tls_index);
}

template<typename T>
void ThreadLocalStorage<T>::SetValue(const T* value) {
  TlsSetValue(tls_index, value);
}

template<typename T>
T* ThreadLocalStorage<T>::GetValue() {
  return TlsGetValue(tls_index);
}

