#pragma once
#include "CameraRotator.h"

namespace camera {
class SimpleCameraRotator: public CameraRotator {
public:
  SimpleCameraRotator() :angle_(0), current_pos_(0), is_running_(false)  {}
  virtual bool RotateNext(CameraRotationPos* pos) override {
    current_pos_ = CameraRotationPos(angle_);
    if (pos)  *pos = current_pos_;
    if (angle_ >= kMaxAngle) {
      angle_ -= kMaxAngle;
      is_running_ = false;
      return false;
    }
    is_running_ = true;
    angle_ += kAnglePerRotation;
    return true;
  }

  void Reset() {
    angle_ = 0;
    current_pos_ = 0;
    is_running_ = false;
  }

  virtual bool IsRunning() {
    return is_running_;
  }

  virtual CameraRotationPos GetRotationPos() {
    return current_pos_;
  }
private:
  bool is_running_;
  CameraRotationPos current_pos_;
  int angle_;
  const static int kMaxAngle = 360;
  const static int kAnglePerRotation = 15;
};
}