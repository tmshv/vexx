#ifndef GCRENDERTOSCREEN_H
#define GCRENDERTOSCREEN_H

#include "GCGlobal.h"
#include "GCRenderTarget.h"

class GRAPHICSCORE_EXPORT GCScreenRenderTarget : public GCRenderTarget
  {
  S_ENTITY(GCScreenRenderTarget, GCRenderTarget, 0)

public:
  GCScreenRenderTarget();
  };

#endif // GCRENDERTOSCREEN_H
