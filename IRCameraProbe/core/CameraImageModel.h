#pragma once
#include "IRCameraBasic.h"
#include <functional>

namespace camera {

class CameraImageModel {
public:
  CameraImageModel();

  //sample button is used to sample the camera image
  bool IsSampleButtonEnabled() const  { return is_sample_button_enabled;   }
  //enable or disable sample button
  //enabled is true, then enable sample button,else disable
  void EnableSampleButton(bool enabled);
  //the user click the sample button
  void SampleButtonClicked();

  typedef std::function<void(bool)> ConnectResultObserver;
  void Connect(const ConnectResultObserver& observer);

  bool IsConnected() const;
  void Disconnect();

  void UpdateImage();
private:
  bool is_sample_button_enabled;
};
}