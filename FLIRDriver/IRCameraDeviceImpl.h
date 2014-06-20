
#pragma  once
#include "IRCameraDevice.h"

class FakeDialog;
class CLvcamctrl;
class InternalEventHandler;
class FakeDialogEventObserver;

class IRCameraDeviceImpl : public IRCameraDevice {
public:
  IRCameraDeviceImpl();
  ~IRCameraDeviceImpl();

  /**
  * connect to the camera, Return is the status value
  * when the return status code is 0, then the invoke success,
  * otherwise can invoke IRCamera_GetError(code) to get the error string
  */
  IRCameraStatusCode Connect(const TString& ip_addr);
  /**
  * disconnect with the camera, Return is the status code
  */
  IRCameraStatusCode Disconnect();
  /**
  * get the error string by the status code
  * @param buffer: the buffer to fill the error string
  * @param str_len: the size of buffer in TCHAR ( number of element)
  */
  TString  GetErrorString(IRCameraStatusCode);

  /**
  * get the width of image
  */
  int   GetImageWidth();
  /**
  * get the height of image
  */
  int   GetImageHeight();

  /**
  * get the image that contain the kelvin temperature
  * @img_filling: the array of width*height elements, to fill the temperature value for every pixel
  */
  IRCameraStatusCode GetKelvinImage(IRCameraImageFilling* img_filling);

  void  RegisterEventHandler(IRCameraEventHandler*);

  IRCameraDeviceStatus  GetStatus();
private:
  FakeDialog*   fake_dlg_;
  CLvcamctrl*   camera_;
  IRCameraDeviceStatus camera_status_;

  int image_width_;
  int image_height_;
  IRCameraEventHandler* event_handler_; //<the event handler that user register
  FakeDialogEventObserver* internal_event_handler_;
  friend class InternalEventHandler;
};
