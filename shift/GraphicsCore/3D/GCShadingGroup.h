#ifndef GCSHADINGGROUP_H
#define GCSHADINGGROUP_H

#include "GCGlobal.h"
#include "GCRenderable.h"
#include "sbasepointerproperties.h"
#include "GCShader.h"
#include "Manipulators/GCManipulator.h"

class GRAPHICSCORE_EXPORT GCShadingGroup : public GCRenderArray, public GCManipulatable
  {
  S_ENTITY(GCShadingGroup, GCRenderArray, 0)

public:
  GCShadingGroup();

  void render(XRenderer *r) const;

  virtual void addManipulators(SPropertyArray *, const GCTransform *tr=0);

  GCShaderPointer shader;
  };

S_PROPERTY_INTERFACE(GCShadingGroup)

S_TYPED_POINTER_TYPE(GCShadingGroupPointer, GCShadingGroup);

S_TYPED_POINTER_ARRAY_TYPE(GCShadingGroupPointerArray, GCShadingGroupPointer);

#endif // GCSHADINGGROUP_H
