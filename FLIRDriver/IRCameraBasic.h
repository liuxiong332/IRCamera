#pragma once

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