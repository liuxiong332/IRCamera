#include "stdafx.h"
#include "IRCameraDeviceMock.h"

namespace {
  const int kImageWidth = 320;
  const int kImageHeight = 240;

  const float kKelvinTempTransform = 273.15f;
  const float kTempMinValue = kKelvinTempTransform + 20;
  const float kTempMaxValue = kKelvinTempTransform + 40;

  const int kImageArraySize = 1;
  float kImageArray[kImageWidth*kImageHeight];
}


IRCameraDeviceMock::IRCameraDeviceMock() {
  device_status_ = IRCAMERA_DISCONNECTED;
  for (int j = 0; j < kImageWidth*kImageHeight; ++j) {
    kImageArray[j] = rand() % static_cast<int>(kTempMaxValue - kTempMinValue)
      + kTempMinValue;
  }
  event_handler_ = NULL;
  image_index_ = 0;
}


IRCameraDeviceMock::~IRCameraDeviceMock() {
}

IRCameraStatusCode  IRCameraDeviceMock::Connect(const TString&) {
  device_status_ = IRCAMERA_CONNECTED;
  if (event_handler_)
    event_handler_->OnEvent(IRCAMERA_CONNECTED_EVENT);
  return IRCAMERA_OK;
}

IRCameraStatusCode  IRCameraDeviceMock::Disconnect() {
  device_status_ = IRCAMERA_DISCONNECTED;
  if (event_handler_) {
    event_handler_->OnEvent(IRCAMERA_DISCONNECTED_EVENT);
  }
  return IRCAMERA_OK;
}

TString IRCameraDeviceMock::GetErrorString(IRCameraStatusCode code) {
  return _T("Nothing");
}

int IRCameraDeviceMock::GetImageWidth() {
  return kImageWidth;
}

int IRCameraDeviceMock::GetImageHeight() {
  return kImageHeight;
}

IRCameraStatusCode IRCameraDeviceMock::GetKelvinImage(IRCameraImageFilling* img_filling) {
  img_filling->SetBuffer(kImageArray , kImageWidth*kImageHeight);
  image_index_ = (++image_index_) % kImageArraySize;
  return IRCAMERA_OK;
}

void IRCameraDeviceMock::RegisterEventHandler(IRCameraEventHandler* handler) {
  event_handler_ = handler;
}

IRCameraDeviceStatus  IRCameraDeviceMock::GetStatus() {
  return device_status_;
}