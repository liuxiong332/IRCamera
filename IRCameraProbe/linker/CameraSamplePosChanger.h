#pragma once

namespace camera {
//CameraSamplePosChanger is used for changing the position of camera and
//get the temperature information for all of the direction
class CameraSamplePosChanger {
  virtual bool NextPos() = 0;
  virtual ~CameraSamplePosChanger() {}
};

}