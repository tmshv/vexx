#ifndef GCRENDERTOSCREEN_H
#define GCRENDERTOSCREEN_H

#include "GCGlobal.h"
#include "GCRenderTarget.h"

class GRAPHICSCORE_EXPORT GCRenderToScreen : public GCRenderTarget
  {
  S_ENTITY(GCRenderToScreen, GCRenderTarget, 0)

public:
  GCRenderToScreen();

  void render();
  };

#endif // GCRENDERTOSCREEN_H
