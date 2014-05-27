#pragma once
#include "SkWindow.h"

class GrContext;
class GrRenderTarget;
class AttachmentInfo;
struct GrGLInterface;

class SkGLWindow: public SkOSWindow
{
public:
  SkGLWindow(void* hwnd);
  ~SkGLWindow();

  bool setupBackend();
  void tearDownBackend();

protected:
  virtual void draw(SkCanvas* canvas) SK_OVERRIDE;

  virtual void onSizeChange() SK_OVERRIDE;

  virtual void onHandleInval(const SkIRect&) SK_OVERRIDE;

  virtual SkCanvas* createCanvas() SK_OVERRIDE;
private:
  void  setupRenderTarget();

  GrContext*  fContext;
  GrRenderTarget* fRenderTarget;
  AttachmentInfo  fAttachmentInfo;
  const GrGLInterface*  fInterface;

};

