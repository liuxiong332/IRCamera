#pragma once

class CameraImageBuffer {
public:
  virtual int GetWidth() = 0;
  virtual int GetHeight() = 0;
  virtual float* GetBuffer() = 0;

  virtual ~CameraImageBuffer()  {}
};