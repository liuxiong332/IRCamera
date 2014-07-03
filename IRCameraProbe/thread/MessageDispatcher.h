#pragma once
#include <windows.h>

namespace camera {
class MessageDispatcher {
public:
  virtual void Dispatch(const MSG& msg) = 0;
  virtual ~MessageDispatcher() {}
};
}