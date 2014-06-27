#include "stdafx.h"
#include "DIBitmap.h"


DIBitmap::DIBitmap() :bitmap_header_(NULL), bitmap_buffer_(NULL), bitmap_palette_(NULL) {}
DIBitmap::~DIBitmap() {
  if (bitmap_header_)
    free(bitmap_header_);
}

void DIBitmap::Init(int width, int height) {
  if (bitmap_header_ && img_width_ == width && img_height_ == height)
    return;
  img_width_ = width;
  img_height_ = height;

  int palette_size = PALETTE_COUNT * sizeof(RGBQUAD);  //the size of palette
  int align_width = (width + 3)&~3;   //4 byte align
  int img_size = sizeof(BITMAPINFOHEADER);
  img_size += palette_size;
  img_size += align_width * height;

  if (bitmap_header_) 
    free(bitmap_header_); 
  bitmap_header_ = (BITMAPINFOHEADER*)malloc(img_size);
  bitmap_buffer_ = (BYTE*)bitmap_header_ + sizeof(BITMAPINFOHEADER)+palette_size;

  bitmap_header_->biSize = sizeof(BITMAPINFOHEADER);
  bitmap_header_->biWidth = width;
  bitmap_palette_ = (RGBQUAD*)(((LPBITMAPINFO)bitmap_header_)->bmiColors);
  // Specifies the height of the bitmap, in pixels.
  // If biHeight is positive, the bitmap is a bottom-up DIB and
  // its origin is the lower-left corner. If biHeight is negative,
  // the bitmap is a top-down DIB and its origin is the upper-left corner.
  bitmap_header_->biHeight = -height;
  bitmap_header_->biPlanes = 1;
  bitmap_header_->biBitCount = 8;
  bitmap_header_->biCompression = BI_RGB;
  bitmap_header_->biSizeImage = align_width * height;
  bitmap_header_->biXPelsPerMeter = 0;
  bitmap_header_->biYPelsPerMeter = 0;
  bitmap_header_->biClrUsed = 256;
  bitmap_header_->biClrImportant = 0;

  InitPalette();
}

BITMAPINFO* DIBitmap::GetInfo() {
  return reinterpret_cast<BITMAPINFO*>(bitmap_header_);
}

void DIBitmap::InitPalette() {

  int palette_count = GetPaletteLen();
  RGBQUAD * palette_buffer = GetPalettePtr(); 

  int delta = palette_count / 3;
  //green color from the min to the max
  for (int i = 0; i < delta; ++i) {
    palette_buffer[i] = {
      0, 256 * i / delta, 0, 0
    };
  }
  //blue color from the min to the max
  for (int i = 0; i < delta; ++i) {
    palette_buffer[delta + i] = {
      0, 255, 256 * i / delta, 0
    };
  }
  int remain_len = 256 - 2 * delta;;
  //red color from the min to the max
  for (int i = 0; i < remain_len; ++i) {
    palette_buffer[2 * delta + i] = {
      256 * i / remain_len, 255, 255, 0
    };
  }
}
 
int DIBitmap::GetPaletteLen() const {
  return PALETTE_COUNT;
}
RGBQUAD*  DIBitmap::GetPalettePtr() {
  return bitmap_palette_;
}

BYTE* DIBitmap::GetBitBuffer(){
  return bitmap_buffer_;
}

void  DIBitmap::Paint(HDC hdc, RECT rcItem) {
  int old_mode = SetStretchBltMode(hdc, COLORONCOLOR);
  ::StretchDIBits(hdc, rcItem.left, rcItem.top, rcItem.right - rcItem.left,
    rcItem.bottom - rcItem.top,
    0, 0, img_width_, img_height_, GetBitBuffer(), GetInfo(),
    DIB_RGB_COLORS, SRCCOPY);
  SetStretchBltMode(hdc, old_mode);
}