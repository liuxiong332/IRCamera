#pragma once
#include "CameraRotator.h"

namespace camera {
class SimpleCameraRotator: public CameraRotator {
public:
  SimpleCameraRotator() :angle_(0)  {}
  virtual bool RotateNext(CameraRotationPos* pos) override {
    *pos = CameraRotationPos(angle_);
    if (angle_ >= kMaxAngle) {
      angle_ -= kMaxAngle;
      return false;
    }
    angle_ += kAnglePerRotation;
    return true;
  }

  void Reset() {
    angle_ = 0;
  }
private:
  int angle_;
  const static int kMaxAngle = 360;
  const static int kAnglePerRotation = 15;
};
}