#pragma once
#include "IRCameraBasic.h"
#include <tchar.h>

class StringUtility {
public:
  static TString IntToString(int num) {
    TCHAR sz[10];
    swprintf_s(sz, ARRAYSIZE(sz), _T("%d"), num);
    return TString(sz);
  }

  static TString FloatToString(float num) {
    TCHAR sz[10];
    swprintf_s(sz, ARRAYSIZE(sz), _T("%.2f"), num);
    return TString(sz);
  }
};