#include "CameraBufferAnalyzer.h"
#include "CameraImageBuffer.h"
#include <algorithm>

namespace camera {
CameraBufferAnalyzer::CameraBufferAnalyzer(CameraImageBuffer* buffer) {
  buffer_ = buffer;
  float* img_buf = buffer->GetBuffer();
  aver_temp_ = 0;
  max_temp_ = *img_buf;

  int width = buffer->GetWidth(), height = buffer->GetHeight();
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      float cur_temp = img_buf[i*width + j];
      aver_temp_ += cur_temp;
      max_temp_ = std::max(max_temp_, cur_temp);
    }
  }
  aver_temp_ /= (width*height);
}

}