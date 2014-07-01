#pragma once

class CameraImageContainerUIObserver {
public:
  virtual void OnConnectButtonClicked() = 0;
  virtual void OnDisconnectButtonClicked() = 0;
  virtual void OnSampleButtonClicked() = 0;
protected:
  virtual ~CameraImageContainerUIObserver() {}
};