#pragma once

class CameraImageListUIObserver {
public:
  virtual void  OnTimer() = 0;

  virtual void OnStableSampleTimer() = 0;

  virtual ~CameraImageListUIObserver() {}
};