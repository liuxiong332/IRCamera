#pragma once

namespace camera {
class CameraRotationPos {
public:
  CameraRotationPos(int angle = 0) : angle_(angle) {}
  int GetAngle() const {
    return angle_;
  }

  void SetAngle(int angle) {
    angle_ = angle;
  }
private:
  int angle_;
};
//the rotator that make the camera rotate around
class CameraRotator {
public:
  //rotate to the next position, the return value is pos after rotation
  virtual bool RotateNext(CameraRotationPos*) = 0;

  virtual void Reset() = 0;

  virtual bool IsRunning() = 0;
  virtual CameraRotationPos GetRotationPos() = 0;
  virtual ~CameraRotator() {}
};
}