#ifndef GCVIEWPORT_H
#define GCVIEWPORT_H

#include "GCGlobal.h"
#include "sbaseproperties.h"
#include "GCScreenRenderTarget.h"

class GRAPHICSCORE_EXPORT GCViewport : public GCScreenRenderTarget
  {
  S_ENTITY(GCViewport, GCScreenRenderTarget, 0)

public:
  GCViewport();

  UnsignedIntProperty x;
  UnsignedIntProperty y;
  };

S_PROPERTY_INTERFACE(GCViewport)

#endif // GCVIEWPORT_H
