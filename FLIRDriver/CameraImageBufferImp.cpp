#include "stdafx.h"
#include "CameraImageBufferImp.h"

CameraImageBufferImp::CameraImageBufferImp(HGLOBAL h, int width, int height)
    : width_(width), height_(height), hGlobal_(h) {
  buffer_ = (float*)GlobalLock(hGlobal_);
}

CameraImageBufferImp::~CameraImageBufferImp() {
  GlobalUnlock(hGlobal_);
  GlobalFree(hGlobal_);
}