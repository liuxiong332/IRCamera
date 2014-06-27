#pragma once

class CameraImageBuffer {
public:
  virtual int GetWidth() const = 0;
  virtual int GetHeight() const = 0;
  virtual float* GetBuffer() = 0;

  virtual ~CameraImageBuffer()  {}
};