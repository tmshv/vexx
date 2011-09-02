#ifndef GCGEOMETRYTRANSFORM_H
#define GCGEOMETRYTRANSFORM_H

#include "GCGlobal.h"
#include "sbasepointerproperties.h"
#include "GCTransform.h"
#include "GCGeometry.h"
#include "3D/Manipulators/GCManipulator.h"

class GRAPHICSCORE_EXPORT GCGeometryTransform : public GCTransform, public GCManipulatable
  {
  S_PROPERTY(GCGeometryTransform, GCTransform, 0)
public:
  GCGeometryTransform();

  GCGeometryPointer geometry;

  void render(XRenderer* r) const;

  void addManipulators(SPropertyArray *);
  };

S_TYPED_POINTER_TYPE(GCGeometryTransformPointer, GCGeometryTransform)

S_TYPED_POINTER_ARRAY_TYPE(GCGeometryTransformPointerArray, GCGeometryTransformPointer)

#endif // GCGEOMETRYTRANSFORM_H
