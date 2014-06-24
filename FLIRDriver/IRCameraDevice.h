/** 
 *  FLIR Camera interface, this interface wrap the FLIR SDK ACTIVEX interface
 */
#ifndef IRCAMERADRIVER_H_
#define IRCAMERADRIVER_H_

#include "IRCameraBasic.h"

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

class IRCameraDeviceListManager;
class CameraImageBuffer;

class IRCAMERA_API IRCameraDevice {
public:
  virtual ~IRCameraDevice() {}

  /**
  * connect to the camera, Return is the status value
  * when the return status code is 0, then the invoke success,
  * otherwise can invoke IRCamera_GetError(code) to get the error string
  */
  virtual IRCameraStatusCode Connect(const TString& ip_addr) = 0;
  /**
  * disconnect with the camera, Return is the status code
  */
  virtual IRCameraStatusCode Disconnect() = 0;
  /**
  * get the error string by the status code
  * @param buffer: the buffer to fill the error string
  * @param str_len: the size of buffer in TCHAR ( number of element)
  */
  virtual TString  GetErrorString(IRCameraStatusCode) = 0;

  /**
  * get the width of image
  */
  virtual int   GetImageWidth() = 0;
  /**
  * get the height of image
  */
  virtual int   GetImageHeight() = 0;

  /**
  * get the image that contain the kelvin temperature
  * @img_filling: the array of width*height elements, to fill the temperature value for every pixel
  */
  virtual IRCameraStatusCode GetKelvinImage(CameraImageBuffer** buffer) = 0;

  /**
  * register the camera event handler
  * IRCameraOnEvnet is the handler to the camera event
  */
  class IRCameraEventHandler {
  public:
    virtual void OnEvent(IRCameraEvent event_type) = 0;
    virtual ~IRCameraEventHandler() {}
  };
  virtual void  RegisterEventHandler(IRCameraEventHandler*) = 0;

  virtual IRCameraDeviceStatus  GetStatus() = 0;

  static  IRCameraDevice* Create();
  static  IRCameraDevice* CreateForTest();
private:
  friend class IRCameraDeviceListManager;
   
};

#endif