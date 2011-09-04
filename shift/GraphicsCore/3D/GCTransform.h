#ifndef GCTRANSFORM_H
#define GCTRANSFORM_H

#include "GCGlobal.h"
#include "sentity.h"
#include "XTransform.h"
#include "sbaseproperties.h"
#include "3D/Manipulators/GCManipulator.h"
#include "GCBaseProperties.h"

class GRAPHICSCORE_EXPORT GCTransform : public SEntity, public GCManipulatable
  {
  S_ENTITY(GCTransform, SEntity, 0)

public:
  GCTransform();

  TransformProperty transform;

  virtual void addManipulators(SPropertyArray *, const GCTransform *tr=0);
  };

#endif // GCTRANSFORM3D_H
