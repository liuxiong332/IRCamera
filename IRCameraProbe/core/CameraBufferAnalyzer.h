#pragma once
#include "IRCameraBasic.h"
#include "CameraRotator.h"

class CameraImageBuffer;
namespace camera {
class CameraBufferAnalyzer {
public:
  void AddCameraBuffer(const TString& camera_ip, const CameraRotationPos& pos, CameraImageBuffer* buffer);
};
}