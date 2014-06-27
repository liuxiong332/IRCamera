#pragma once

class CameraSampleAction {
public:
  virtual void  SampleImage() = 0;
  virtual ~CameraSampleAction() {}
};