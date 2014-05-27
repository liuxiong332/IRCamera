#include "stdafx.h"
#include "IRCameraTest.h"
#include <assert.h>

IRCameraTest::IRCameraTest(IRCameraEventHandler handler)
{
  camera_info_ = IRCameraCreate();
  IRCameraRegisterEventHandler(camera_info_, handler);
  IRCameraStatusCode code = IRCameraConnect(camera_info_);
  assert(code == IRCamera_OK);
}


IRCameraTest::~IRCameraTest()
{
  IRCameraDisconnect(camera_info_);
  IRCameraDestroy(camera_info_);
}

void IRCameraTest::Paint(HWND hWnd)
{
  HDC hdc = GetDC(hWnd);
  int img_width = IRCameraGetImageWidth(camera_info_);
  int img_height = IRCameraGetImageHeight(camera_info_);
  float* temp_buffer = (float*)malloc(img_width*img_height*(sizeof(float)));
  
  IRCameraStatusCode code = IRCameraGetKelvinImage(camera_info_, temp_buffer);
  if (code != IRCamera_OK) {
    return;
  }

  int header_size = sizeof(BITMAPINFOHEADER)+256 * sizeof(RGBQUAD);
  int img_size = header_size + img_width*img_height;
  BITMAPINFOHEADER* pBuffer = (BITMAPINFOHEADER*)malloc(img_size);
  BYTE* img_buffer = (BYTE*)pBuffer + header_size;
  assert(pBuffer != NULL);

  // Set bitmap info header
  BITMAPINFOHEADER   bihS;  // Temporary structure
  LPBITMAPINFOHEADER bitHeadP = (LPBITMAPINFOHEADER)pBuffer;

  bihS.biSize = sizeof(BITMAPINFOHEADER);
  bihS.biWidth = img_width;
  // Specifies the height of the bitmap, in pixels.
  // If biHeight is positive, the bitmap is a bottom-up DIB and
  // its origin is the lower-left corner. If biHeight is negative,
  // the bitmap is a top-down DIB and its origin is the upper-left corner. 
  bihS.biHeight = -img_height;
  bihS.biPlanes = 1;
  bihS.biBitCount = 8;
  bihS.biCompression = BI_RGB;
  bihS.biSizeImage = img_width * img_height;
  bihS.biXPelsPerMeter = 0;
  bihS.biYPelsPerMeter = 0;
  bihS.biClrUsed = 256;
  bihS.biClrImportant = 0;
  *bitHeadP = bihS;  // Copy struct into DIB

  // Set palette (grey scale)
  RGBQUAD *palP = (RGBQUAD*)&((LPBITMAPINFO)pBuffer)->bmiColors[0];
  for (WORD i = 0; i < 256; i++) {
    palP->rgbBlue = (BYTE)i;
    palP->rgbGreen = (BYTE)i;
    palP->rgbRed = (BYTE)i;
    palP->rgbReserved = 0;
    palP++;
  }

  RECT  rect;
  GetClientRect(hWnd, &rect);
  int old_mode = SetStretchBltMode(hdc, COLORONCOLOR);

  int ret = ::StretchDIBits(hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
    0, 0, img_width, img_height, img_buffer, (LPBITMAPINFO)pBuffer, DIB_RGB_COLORS, SRCCOPY);
  SetStretchBltMode(hdc, old_mode);

  delete temp_buffer;
  ReleaseDC(hWnd, hdc);
}
