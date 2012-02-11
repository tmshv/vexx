#ifndef SYPREVIEWVIEWPORT_H
#define SYPREVIEWVIEWPORT_H

#include "UISurface.h"
#include "X2DCanvas.h"
#include "XAbstractDelegate.h"
#include "GCShiftRenderModel.h"
#include "sentityweakpointer.h"
#include "3D/gcviewport.h"
#include "QImage"

class X2DCanvas;
class SyImageInput;

class SyPreviewViewport : public GCViewport
  {
  S_ENTITY(SyPreviewViewport, GCViewport, 0)

public:
  SyPreviewViewport();

  void setupViewport(const SyImageInput *, XAbstractCanvasController **controller = 0);
  };

#endif // SYPREVIEWVIEWPORT_H
