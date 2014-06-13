#pragma once
#include "IRCameraBasic.h"
/** 
 * the observer that notified when the connectivity changed
 */
class IRCAMERA_API ConnectivityChangeObserver {
public:
  virtual void OnConnectivityChanged() = 0;
  virtual ~ConnectivityChangeObserver() {}
};