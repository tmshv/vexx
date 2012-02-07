#ifndef GCPLANE_H
#define GCPLANE_H

#include "sentity.h"
#include "3D/GCGeometry.h"
#include "3D/GCTransform.h"
#include "3D/Manipulators/GCManipulator.h"

class GRAPHICSCORE_EXPORT GCPlane : public SEntity, public GCManipulatable
  {
  S_PROPERTY(GCPlane, SEntity, 0)

public:
  GCPlane();

  FloatProperty width;
  FloatProperty height;

  GCGeometry geometry;

  void addManipulators(SPropertyArray *, const GCTransform *tr=0);
  };

#endif // GCPLANE_H
