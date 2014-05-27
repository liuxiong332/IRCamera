#include "SkGLWindow.h"

#include "gl/GrGLUtil.h"
#include "gl/GrGLDefines.h"
#include "gl/GrGLInterface.h"
#include "SkApplication.h"
#include "SkGpuDevice.h"
#include "SkGraphics.h"


SkGLWindow::SkGLWindow(void * hwnd) :SkOSWindow(hwnd)
{
}


SkGLWindow::~SkGLWindow()
{
}

void  SkGLWindow::tearDownBackend() {
  SkSafeUnref(fContext);
  SkSafeUnref(fInterface);
  SkSafeUnref(fRenderTarget);
}

bool SkGLWindow::setupBackend() {
  this->setColorType(kRGBA_8888_SkColorType);
  this->setClipToBounds(false);
  this->setVisibleP(true);

  bool result = attach(kNativeGL_BackEndType, 0, &fAttachmentInfo);
  if (false == result) {
    SkDebugf("Not possible to create backend\n");
    detach();
    return false;
  }

  fInterface = GrGLCreateNativeInterface();
  SkASSERT(NULL != fInterface);

  fContext = GrContext::Create(kOpenGL_GrBackend, (GrBackendContext)fInterface);
  SkASSERT(NULL != fContext);

  setupRenderTarget();

  return true;
}


void SkGLWindow::setupRenderTarget() {
  GrBackendRenderTargetDesc desc;
  desc.fWidth = SkScalarRoundToInt(width());
  desc.fHeight = SkScalarRoundToInt(height());
  desc.fConfig = kSkia8888_GrPixelConfig;
  desc.fOrigin = kBottomLeft_GrSurfaceOrigin;
  desc.fSampleCnt = fAttachmentInfo.fSampleCount;
  desc.fStencilBits = fAttachmentInfo.fStencilBits;

  GrGLint buffer;
  GR_GL_GetIntegerv(fInterface, GR_GL_FRAMEBUFFER_BINDING, &buffer);
  desc.fRenderTargetHandle = buffer;

  fRenderTarget = fContext->wrapBackendRenderTarget(desc);

  fContext->setRenderTarget(fRenderTarget);
}

SkCanvas* SkGLWindow::createCanvas() {
  SkAutoTUnref<SkBaseDevice> device(new SkGpuDevice(fContext, fRenderTarget));
  return new SkCanvas(device);
}

void SkGLWindow::draw(SkCanvas* canvas) {
  SkASSERT(NULL != fContext);
  fContext->flush();
  SkOSWindow::present();
}

void SkGLWindow::onSizeChange() {
  setupRenderTarget();
}

void SkGLWindow::onHandleInval(const SkIRect& rect) {
  RECT winRect;
  winRect.top = rect.top();
  winRect.bottom = rect.bottom();
  winRect.right = rect.right();
  winRect.left = rect.left();
  InvalidateRect((HWND)this->getHWND(), &winRect, false);
}