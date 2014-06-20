#pragma once
#include "IRCameraDevice.h"

class IRCameraDeviceMock: public IRCameraDevice
{
public:
  IRCameraDeviceMock();
  ~IRCameraDeviceMock();

  /**
  * connect to the camera, Return is the status value
  * when the return status code is 0, then the invoke success,
  * otherwise can invoke IRCamera_GetError(code) to get the error string
  */
  virtual IRCameraStatusCode Connect(const TString& ip_addr);
  /**
  * disconnect with the camera, Return is the status code
  */
  virtual IRCameraStatusCode Disconnect();
  /**
  * get the error string by the status code
  * @param buffer: the buffer to fill the error string
  * @param str_len: the size of buffer in TCHAR ( number of element)
  */
  virtual TString  GetErrorString(IRCameraStatusCode);

  /**
  * get the width of image
  */
  virtual int   GetImageWidth();
  /**
  * get the height of image
  */
  virtual int   GetImageHeight();

  /**
  * get the image that contain the kelvin temperature
  * @img_filling: the array of width*height elements, to fill the temperature value for every pixel
  */
  virtual IRCameraStatusCode GetKelvinImage(IRCameraImageFilling* img_filling);

  virtual void  RegisterEventHandler(IRCameraEventHandler*);

  virtual IRCameraDeviceStatus  GetStatus();
private:
  IRCameraDeviceStatus    device_status_;
  IRCameraEventHandler*   event_handler_;
  int image_index_;
};

