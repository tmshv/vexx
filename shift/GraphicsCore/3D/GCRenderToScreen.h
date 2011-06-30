#ifndef GCRENDERTOSCREEN_H
#define GCRENDERTOSCREEN_H

#include "GCRenderTarget.h"

class GCRenderToScreen : public GCRenderTarget
  {
  S_ENTITY(GCRenderToScreen, GCRenderTarget, 0)

public:
    GCRenderToScreen();
  };

#endif // GCRENDERTOSCREEN_H
