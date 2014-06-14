/** 
 *  FLIR Camera interface, this interface wrap the FLIR SDK ACTIVEX interface
 */
#ifndef IRCAMERADRIVER_H_
#define IRCAMERADRIVER_H_

#ifdef FLIRDRIVER_EXPORT 
#define IRCAMERA_API  extern "C" __declspec(dllexport) 
#else
#define IRCAMERA_API extern "C" __declspec(dllimport) 
#endif
#include <wtypes.h>
typedef int IRCameraStatusCode;
/**the status code 0 stand for OK*/
#define IRCAMERA_OK 0
#define IRCAMERA_NOTPRESENT_ERROR 1   //the device is not present

struct IRCameraInfo;
/** 
 * create the new IRCameraInfo object
 */
IRCAMERA_API IRCameraInfo* IRCameraCreate();
/** 
 * destroy the specific IRCameraInfo object
 */
IRCAMERA_API void  IRCameraDestroy(IRCameraInfo*);
/** 
 * connect to the camera, Return is the status value
 * when the return status code is 0, then the invoke success, 
 * otherwise can invoke IRCamera_GetError(code) to get the error string
 */
IRCAMERA_API  IRCameraStatusCode IRCameraConnect(IRCameraInfo*, LPCTSTR ip_addr);

/** 
 * disconnect with the camera, Return is the status code
 */
IRCAMERA_API  IRCameraStatusCode IRCameraDisconnect(IRCameraInfo* );

/** 
 * get the error string by the status code
 * @param buffer: the buffer to fill the error string
 * @param str_len: the size of buffer in TCHAR ( number of element)
 */
IRCAMERA_API  void  IRCameraGetError(IRCameraInfo*, IRCameraStatusCode, LPTSTR buffer, int str_len);
 
/** 
 * get the width of image
 */
IRCAMERA_API  int   IRCameraGetImageWidth(IRCameraInfo*);
/** 
 * get the height of image
 */
IRCAMERA_API  int  IRCameraGetImageHeight(IRCameraInfo*);

/** 
 * get the image that contain the kelvin temperature
 * @img_filling: the array of width*height elements, to fill the temperature value for every pixel
 */ 
struct IRCameraImageFilling {
  typedef void(*SetBuffer)(void* args, float* val, int val_count);
  SetBuffer   set_buffer;   //the function that set the pixel of pos (x,y)
  void*   args;
};
IRCAMERA_API  IRCameraStatusCode IRCameraGetKelvinImage(IRCameraInfo*, IRCameraImageFilling* img_filling);

/** 
 * register the camera event handler
 * IRCameraOnEvnet is the handler to the camera event
 */
#define  IRCAMERA_CONNECTED_EVENT  2
#define  IRCAMERA_DISCONNECTED_EVENT 3
#define  IRCAMERA_CONNECTBROKEN_EVENT 4
#define  IRCAMERA_RECONNECTED_EVENT 5

typedef void(*IRCameraEventHandler)(int event_type, void* args);
IRCAMERA_API  void  IRCameraRegisterEventHandler(IRCameraInfo*, IRCameraEventHandler, void* args);

#define IRCAMERA_CONNECTED 1
#define IRCAMERA_DISCONNECTED 2
#define IRCAMERA_BROKEN_CONNECTE 3
#define IRCAMERA_RECONNECTED 4
IRCAMERA_API int  IRCameraGetStatus(IRCameraInfo* );
#endif