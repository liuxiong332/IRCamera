/** 
 *  FLIR Camera interface, this interface wrap the FLIR SDK ACTIVEX interface
 */
#ifndef IRCAMERADRIVER_H_
#define IRCAMERADRIVER_H_

#ifdef FLIRDRIVER_EXPORT 
#define IRCAMERA_API  __declspec(dllexport) 
#else
#define IRCAMERA_API  __declspec(dllimport) 
#endif

#include <wtypes.h>
#include <string>

#ifdef _UNICODE
#define   TString  std::wstring
#else
#define   TString  std::string
#endif

enum IRCameraStatusCode {
  /**the status code 0 stand for OK*/
  IRCAMERA_OK,
  IRCAMERA_NOTPRESENT_ERROR  //the device is not present
};

enum IRCameraEvent {
  IRCAMERA_CONNECTED_EVENT = 2, 
  IRCAMERA_DISCONNECTED_EVENT = 3,
  IRCAMERA_CONNECTBROKEN_EVENT = 4,
  IRCAMERA_RECONNECTED_EVENT = 5,
  IRCAMERA_IMAGESIZE_CHANGE_EVENT = 18
};

enum IRCameraDeviceStatus {
  IRCAMERA_CONNECTED,
  IRCAMERA_DISCONNECTED,
  IRCAMERA_BROKEN_CONNECTE,
  IRCAMERA_RECONNECTED
};

class FakeDialog;
class CLvcamctrl;
class InternalEventHandler;
class FakeDialogEventObserver;

class IRCameraImageFilling {
public:
  virtual void SetBuffer(float* val, int val_count) = 0;
  virtual ~IRCameraImageFilling() {}
};

class IRCAMERA_API IRCameraDevice {
public:
  IRCameraDevice();
  ~IRCameraDevice();
  /**
  * connect to the camera, Return is the status value
  * when the return status code is 0, then the invoke success,
  * otherwise can invoke IRCamera_GetError(code) to get the error string
  */
  IRCameraStatusCode Connect();
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
  * set the width of image
  */
  void  SetImageWidth(int width);
  /**
  * set the height of image
  */
  void   SetImageHeight(int height);
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

  /**
  * register the camera event handler
  * IRCameraOnEvnet is the handler to the camera event
  */
  class IRCameraEventHandler {
  public:
    virtual void OnEvent(IRCameraEvent event_type) = 0;
    virtual ~IRCameraEventHandler() {}
  };
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
#endif