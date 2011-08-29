#ifndef GCSHADINGGROUP_H
#define GCSHADINGGROUP_H

#include "GCGlobal.h"
#include "sentity.h"
#include "sbasepointerproperties.h"
#include "GCShader.h"
#include "GCGeometryTransform.h"
#include "Manipulators/GCManipulator.h"

class GRAPHICSCORE_EXPORT GCShadingGroup : public SEntity, public GCManipulatable
  {
  S_ENTITY(GCShadingGroup, SEntity, 0)
public:
  GCShadingGroup();

  void render(XRenderer *r) const;

  void addManipulators(SPropertyArray *);

  GCShaderPointer shader;
  GCGeometryTransformPointerArray geometry;
  };


S_TYPED_POINTER_TYPE(GCShadingGroupPointer, GCShadingGroup);

S_TYPED_POINTER_ARRAY_TYPE(GCShadingGroupPointerArray, GCShadingGroupPointer);

#endif // GCSHADINGGROUP_H
