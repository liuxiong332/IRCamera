#pragma once
#include <wtypes.h>

namespace camera {
class CameraInfoPrefObserver {
public:
  //notify others the camera info preference has changed
  virtual void CameraInfoPrefChanged() = 0;

  virtual ~CameraInfoPrefObserver() {}
};
}