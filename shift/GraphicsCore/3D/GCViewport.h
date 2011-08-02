#ifndef GCVIEWPORT_H
#define GCVIEWPORT_H

#include "GCGlobal.h"
#include "sbaseproperties.h"

class GRAPHICSCORE_EXPORT GCViewport : public SEntity
  {
  S_ENTITY(GCViewport, SEntity, 0)

public:
  GCViewport();

  UnsignedIntProperty x;
  UnsignedIntProperty y;
  UnsignedIntProperty width;
  UnsignedIntProperty height;

  FloatProperty aspectRatio;
  };

#endif // GCVIEWPORT_H
