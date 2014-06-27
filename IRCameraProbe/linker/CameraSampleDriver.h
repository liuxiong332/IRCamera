#pragma once

class CameraSampleDriver {
public:
  virtual void BeginAction() = 0;
  virtual ~CameraSampleDriver() {}
};