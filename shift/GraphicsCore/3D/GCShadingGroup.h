#ifndef GCSHADINGGROUP_H
#define GCSHADINGGROUP_H

#include "GCGlobal.h"
#include "sentity.h"
#include "sbasepointerproperties.h"
#include "GCShader.h"
#include "GCGeometry.h"

class GRAPHICSCORE_EXPORT GCShadingGroup : public SEntity
  {
  S_ENTITY(GCShadingGroup, SEntity, 0)
public:
  GCShadingGroup();

  void render(XRenderer *r) const;

  GCShaderPointer shader;
  GCGeometryPointerArray geometry;
  GCRuntimeGeometry runtimeGeometry;
  };


S_TYPED_POINTER_TYPE(GCShadingGroupPointer, GCShadingGroup);

S_TYPED_POINTER_ARRAY_TYPE(GCShadingGroupPointerArray, GCShadingGroupPointer);

#endif // GCSHADINGGROUP_H
