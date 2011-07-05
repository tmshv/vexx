#ifndef GCTRANSFORM_H
#define GCTRANSFORM_H

#include "GCGlobal.h"
#include "sentity.h"
#include "XTransform.h"
#include "sbaseproperties.h"

DEFINE_POD_PROPERTY(GRAPHICSCORE_EXPORT, TransformProperty, XTransform, XTransform());

class GRAPHICSCORE_EXPORT GCTransform : public SEntity
  {
  S_ENTITY(GCTransform, SEntity, 0)

public:
  GCTransform();

  TransformProperty transform;
  };

#endif // GCTRANSFORM3D_H
