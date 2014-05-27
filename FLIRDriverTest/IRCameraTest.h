#pragma once

#include "IRCameraDriver.h"
#include <windows.h>

class IRCameraTest
{
public:
  IRCameraTest(IRCameraEventHandler handler);
  ~IRCameraTest();

  void  Paint(HWND hWnd);
private:
  IRCameraInfo*  camera_info_;
};

