#ifndef GCVIEWPORT_H
#define GCVIEWPORT_H

#include "GCGlobal.h"
#include "sbaseproperties.h"
#include "GCRenderTarget.h"

class GRAPHICSCORE_EXPORT GCViewport : public GCRenderTarget
  {
  S_ENTITY(GCViewport, SEntity, 0)

public:
  GCViewport();

  UnsignedIntProperty x;
  UnsignedIntProperty y;
  };

#endif // GCVIEWPORT_H
