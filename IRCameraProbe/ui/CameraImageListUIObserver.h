#pragma once

class CameraImageListUIObserver {
public:
  virtual void  OnTimer() = 0;
  virtual ~CameraImageListUIObserver() {}
};