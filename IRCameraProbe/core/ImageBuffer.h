#pragma once
#include "IRCameraDevice.h"

namespace camera {
class ImageBuffer : public IRCameraImageFilling {
public:
  typedef char BYTE;
  ImageBuffer(int width, int height);
  int GetWidth();
  int GetHeight();
  BYTE* GetBuffer();
private:
  virtual void SetBuffer(float* val, int val_count);

  BYTE* buffer_;
  int  width_;
  int  height_;
};
}