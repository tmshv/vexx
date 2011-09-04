#ifndef GCCUBOID_H
#define GCCUBOID_H

#include "sentity.h"
#include "3D/GCGeometry.h"
#include "3D/GCTransform.h"
#include "3D/Manipulators/GCManipulator.h"

class GRAPHICSCORE_EXPORT GCCuboid : public SEntity, public GCManipulatable
  {
  S_PROPERTY(GCCuboid, SEntity, 0)

public:
  GCCuboid();

  FloatProperty width;
  FloatProperty height;
  FloatProperty depth;

  GCGeometry geometry;

  void addManipulators(SPropertyArray *, const GCTransform *tr=0);
  };

#endif // GCCUBOID_H
