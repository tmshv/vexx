#ifndef GCGEOMETRYTRANSFORM_H
#define GCGEOMETRYTRANSFORM_H

#include "GCGlobal.h"
#include "sbasepointerproperties.h"
#include "GCTransform.h"
#include "GCGeometry.h"

class GRAPHICSCORE_EXPORT GCGeometryTransform : public GCTransform
  {
public:
  GCGeometryTransform();

  GCGeometryPointer geometry;

  void render(XRenderer* r);
  };

S_TYPED_POINTER_TYPE(GCGeometryTransformPointer, GCGeometryTransform)

S_TYPED_POINTER_ARRAY_TYPE(GCGeometryTransformPointerArray, GCGeometryTransformPointer)

#endif // GCGEOMETRYTRANSFORM_H
