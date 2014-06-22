#pragma once

#include <windows.h>

class DIBitmap {
public:
  DIBitmap();
  ~DIBitmap();
  //set the width and height of the camera image
  void Init(int width, int height);
  //get the HBITMAP INFO
  BITMAPINFO* GetInfo();
  int GetPaletteLen() const;
  //set the palette of the image
  void SetPalette(int index, const RGBQUAD&);
  RGBQUAD*  GetPalettePtr();
  //get the bitmap buffer
  BYTE* GetBitBuffer();
  //paint into the specific HDC in the rcItem
  void  Paint(HDC hdc, RECT rcItem);

  void InitPalette();
private:
  int   GetMidColor(int begin, int end, int i);

  int img_width_, img_height_;
  BITMAPINFOHEADER*   bitmap_header_;
  BYTE* bitmap_buffer_;
  RGBQUAD*  bitmap_palette_;
  static const int PALETTE_COUNT = 256;
};