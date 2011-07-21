#ifndef GCSCENE_H
#define GCSCENE_H

#include "GCShadingGroup.h"
#include "GCRenderable.h"
#include "GCCamera.h"

#include "XGeometry.h"
#include "XShader.h"

class GRAPHICSCORE_EXPORT GCScene : public GCRenderable
  {
  S_PROPERTY(GCScene, GCRenderable, 0);

public:
  GCScene();

  ComplexTransformProperty cameraProjection;
  TransformProperty cameraTransform;

  void render(XRenderer *) const;

  GCShadingGroupPointerArray shadingGroups;

private:
  XGeometry x;
  XShader s;
  };

#endif // GCSCENE_H
