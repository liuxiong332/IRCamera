#pragma once

class CameraImageListUIObserver {
public:
  virtual void  OnTimer(int id) = 0;

  virtual ~CameraImageListUIObserver() {}
};