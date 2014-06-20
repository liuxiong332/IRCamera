#include "stdafx.h"
#include "IRCameraDevice.h"
#include "IRCameraDeviceImpl.h"
#include "IRCameraDeviceMock.h"

IRCameraDevice* IRCameraDevice::Create() {
  return new IRCameraDeviceImpl;
}

IRCameraDevice* IRCameraDevice::CreateForTest() {
  return new IRCameraDeviceMock;
}

