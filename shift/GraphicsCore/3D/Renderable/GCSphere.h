#ifndef GCSPHERE_H
#define GCSPHERE_H

#include "sentity.h"
#include "3D/GCGeometry.h"
#include "3D/GCTransform.h"
#include "3D/Manipulators/GCManipulator.h"

class GRAPHICSCORE_EXPORT GCSphere : public SEntity, public GCManipulatable
  {
  S_PROPERTY(GCSphere, SEntity, 0)

public:
  GCSphere();

  FloatProperty radius;

  GCGeometry geometry;

  void addManipulators(SPropertyArray *, const GCTransform *tr=0);
  };

#endif // GCSPHERE_H
