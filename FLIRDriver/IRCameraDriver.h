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
#define IRCamera_OK 0

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
IRCAMERA_API  IRCameraStatusCode IRCameraConnect(IRCameraInfo* );

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
 * @temp_image: the array of width*height elements, to fill the temperature value for every pixel
 */
IRCAMERA_API  IRCameraStatusCode IRCameraGetKelvinImage(IRCameraInfo*, float* temp_image);

/** 
 * register the camera event handler
 * IRCameraOnEvnet is the handler to the camera event
 */
#define  IRCAMERA_CONNECTED_EVENT  2
#define  IRCAMERA_DISCONNECTED_EVENT 3

typedef void(*IRCameraEventHandler)(int event_type);
IRCAMERA_API  void  IRCameraRegisterEventHandler(IRCameraInfo*, IRCameraEventHandler);
#endif