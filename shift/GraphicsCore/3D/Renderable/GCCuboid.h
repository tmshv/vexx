#ifndef GCCUBOID_H
#define GCCUBOID_H

#include "sentity.h"
#include "3D/GCGeometry.h"

class GRAPHICSCORE_EXPORT GCCuboid : public SEntity
  {
  S_PROPERTY(GCCuboid, SEntity, 0)

public:
  GCCuboid();

  FloatProperty width;
  FloatProperty height;
  FloatProperty depth;

  GCGeometry geometry;
  };

#endif // GCCUBOID_H
