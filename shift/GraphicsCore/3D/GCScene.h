#ifndef GCSCENE_H
#define GCSCENE_H

#include "GCRenderable.h"
#include "GCCamera.h"

class GCScene : public GCRenderable
  {
  S_PROPERTY(GCScene, GCRenderable, 0);

public:
  GCScene();

  ComplexTransformProperty cameraProjection;
  TransformProperty cameraTransform;

  void render(XRenderer *) const;
  };

#endif // GCSCENE_H
