#pragma once

namespace camera {
class RotationBufferAnalyzer;
class ImageBufferPusherObserver {
public:
  virtual void OnRotationBufferAnalyzerAvailable(RotationBufferAnalyzer* analyzer) = 0;

  virtual ~ImageBufferPusherObserver() {}
};
}