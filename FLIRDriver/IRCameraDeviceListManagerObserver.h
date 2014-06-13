#pragma once

class IRCameraDevice;
/*
 * the observer to monitor the device list state
 * when adding or removing the device, the observer will be notified
 */
class IRCameraDeviceListManagerObserver {
public:
  virtual void OnDeviceAvailable(IRCameraDevice*) = 0;
  virtual void OnDeviceUnavailable(IRCameraDevice*) = 0;

  virtual ~IRCameraDeviceListManagerObserver()  {}
};