#ifndef GCSHADINGGROUP_H
#define GCSHADINGGROUP_H

#include "sentity.h"
#include "GCShader.h"

class GCShadingGroup : public SEntity
  {
  S_ENTITY(GCShadingGroup, SEntity, 0)
public:
  GCShadingGroup();

  GCShaderPointer shader;
  };


S_TYPED_POINTER_TYPE(GCShadingGroupPointer, GCShadingGroup);

S_TYPED_POINTER_ARRAY_TYPE(GCShadingGroupPointerArray, GCShadingGroupPointer);

#endif // GCSHADINGGROUP_H
