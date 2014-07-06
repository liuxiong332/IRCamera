#pragma once
#include "CameraRotator.h"

namespace camera {
class SimpleCameraRotator: public CameraRotator {
public:
  SimpleCameraRotator() :angle_(0)  {}
  virtual bool RotateNext(CameraRotationPos* pos) override {
    *pos = CameraRotationPos(angle_);
    angle_ += kAnglePerRotation;
    if (angle_ >= kMaxAngle) {
      angle_ -= kMaxAngle;
      return false;
    }
    return true;
  }
private:
  int angle_;
  const static int kMaxAngle = 360;
  const static int kAnglePerRotation = 15;
};
}