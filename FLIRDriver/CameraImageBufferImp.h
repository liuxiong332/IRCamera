#pragma once
#include "CameraImageBuffer.h"
#include <windows.h>

class CameraImageBufferImp : public CameraImageBuffer {
public:
  CameraImageBufferImp(HGLOBAL h, int width, int height);
  virtual int GetWidth() const {
    return width_;
  }
  virtual int GetHeight() const {
    return height_;
  }
  virtual float* GetBuffer() {
    return buffer_;
  }

  ~CameraImageBufferImp();
private:
  int width_;
  int height_;
  HGLOBAL hGlobal_;
  float*  buffer_;
};