#include "CameraImageControl.h"
#include "IRCameraDriver.h"
#include "CameraManage.h"
#include <algorithm>

namespace {
  class CameraImageControlUI : public DuiLib::CControlUI,
    public CameraManage::ConnectStatusObserver
  {
  public:
    CameraImageControlUI(CameraManage* manager);
    ~CameraImageControlUI();

    LPCTSTR GetClass() const;

    virtual void PaintStatusImage(HDC hDC);

    virtual void OnInitCamera() {}
    virtual void  OnConnect();
    virtual void  OnDisconnect();
    virtual void  OnImageUpdate();
  private:
    bool ImageCtrlTimerProc(void* param);

    static void SetBuffer(void* args,  float*, int);
    void InitBitmapHeader();

    int align_width;
    int img_width, img_height;

    float min_temp, max_temp;
    bool  is_need_update_;

    IRCameraImageFilling  image_filling;
    CameraManage* camera_manage;
    BITMAPINFOHEADER*  bitmap_header;
    BYTE*          bitmap_buffer;
  };

  CameraImageControlUI::CameraImageControlUI(CameraManage* manager) :
    camera_manage(manager),bitmap_buffer(NULL)  {
    const static float kKelvinTransform = 273.16f;
    min_temp = kKelvinTransform + 20;
    max_temp = kKelvinTransform + 40;
    bitmap_header = NULL;
    bitmap_buffer = NULL;
    is_need_update_ = false;
    image_filling = { CameraImageControlUI::SetBuffer, this };
   
    OnNotify += MakeDelegate(this, &CameraImageControlUI::ImageCtrlTimerProc);
    camera_manage->AddConnectStatusObserver(this);
  //  InitBitmapHeader();
  }

  bool CameraImageControlUI::ImageCtrlTimerProc(void* param) {
    DuiLib::TNotifyUI*  notify = reinterpret_cast<DuiLib::TNotifyUI*>(param);
    if (notify->sType == _T("timer")) {
      camera_manage->UpdateKelvinImage(&image_filling);
      return true;
    }
    return false;
  }

  CameraImageControlUI::~CameraImageControlUI() {
    OnDisconnect();
    camera_manage->RemoveConnectStatusObserver(this);
  }

  void CameraImageControlUI::OnConnect() {
    InitBitmapHeader();
  }
  
  void CameraImageControlUI::OnImageUpdate() {
    is_need_update_ = true;
    this->Invalidate();
  }
  void CameraImageControlUI::OnDisconnect() {
    if (bitmap_header)
      free(bitmap_header);
    bitmap_header = NULL;
    bitmap_buffer = NULL;
  }

  void CameraImageControlUI::InitBitmapHeader() {
    if (bitmap_header)
      return;
    img_width = camera_manage->GetImageWidth();
    img_height = camera_manage->GetImageHeight();

    const int PALETTE_COUNT = 256;  
    int palette_size = PALETTE_COUNT * sizeof(RGBQUAD);  //the size of palette
    align_width = (img_width + 3)&~3;   //4 byte align
    int img_size = sizeof(BITMAPINFOHEADER);
    img_size += palette_size;
    img_size += align_width * img_height;

    bitmap_header = (BITMAPINFOHEADER*)malloc(img_size);
    bitmap_buffer = (BYTE*)bitmap_header + sizeof(BITMAPINFOHEADER)+ palette_size;

    bitmap_header->biSize = sizeof(BITMAPINFOHEADER);
    bitmap_header->biWidth = img_width;
    // Specifies the height of the bitmap, in pixels.
    // If biHeight is positive, the bitmap is a bottom-up DIB and
    // its origin is the lower-left corner. If biHeight is negative,
    // the bitmap is a top-down DIB and its origin is the upper-left corner.
    bitmap_header->biHeight = -img_height;
    bitmap_header->biPlanes = 1;
    bitmap_header->biBitCount = 8;
    bitmap_header->biCompression = BI_RGB;
    bitmap_header->biSizeImage = align_width * img_height;
    bitmap_header->biXPelsPerMeter = 0;
    bitmap_header->biYPelsPerMeter = 0;
    bitmap_header->biClrUsed = 256;
    bitmap_header->biClrImportant = 0;

    //Init the palette
    RGBQUAD * palette_buffer = (RGBQUAD*)( ((LPBITMAPINFO)bitmap_header)->bmiColors);
    for (int i = 0; i < PALETTE_COUNT; ++i) {
      palette_buffer[i] = { i, i, i ,0 };
    }
  }
  LPCTSTR CameraImageControlUI::GetClass() const {
    return _T("CameraImageUI");
  }

  void CameraImageControlUI::SetBuffer(void* args, float* val, int val_count) {
    CameraImageControlUI* img_control = reinterpret_cast<CameraImageControlUI*>(args);
    BYTE* buffer = img_control->bitmap_buffer;

    float min_temp = (float)INT_MAX, max_temp = (float)INT_MIN;
    for (int i = 0; i < val_count; ++i) {
      min_temp = std::min(min_temp, val[i]);
      max_temp = std::max(max_temp, val[i]);
    }
    float span = max_temp - min_temp;
    int len = 0;
    for (int i = 0; i < img_control->img_height; ++i) {
      for (int j = 0; j < img_control->img_width; ++j) {
        buffer[j] = static_cast<BYTE>((val[len] - min_temp) * 256 / span);
        ++len;
      }
      buffer += img_control->align_width;
    }
    img_control->min_temp = min_temp;
    img_control->max_temp = max_temp;
  }

  void CameraImageControlUI::PaintStatusImage(HDC hDC) {
    if (camera_manage->GetStatus() != CameraManage::CONNECTED || !is_need_update_)
      return;
    int old_mode = SetStretchBltMode(hDC, COLORONCOLOR);
    ::StretchDIBits(hDC,
      m_rcItem.left, m_rcItem.top, m_rcItem.right-m_rcItem.left, m_rcItem.bottom-m_rcItem.top,
      0, 0, img_width, img_height, bitmap_buffer, (LPBITMAPINFO)bitmap_header, DIB_RGB_COLORS, SRCCOPY);
    SetStretchBltMode(hDC, old_mode);
  }
}

/////////////////////////CameraImageControl////////////////////
CameraImageControl::CameraImageControl()
{
  camera_info = NULL;
}

void CameraImageControl::Init(CameraManage* manage) {
  camera_info = manage;
}

CameraImageControl::~CameraImageControl()
{
  
}

DuiLib::CControlUI*  CameraImageControl::CreateControl(LPCTSTR class_name) {
  if (_tcscmp(class_name, _T("CameraImage")) == 0) {
    return new CameraImageControlUI(camera_info);
  }
  return NULL;
}