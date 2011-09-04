#ifndef GCSCENE_H
#define GCSCENE_H

#include "GCShadingGroup.h"
#include "GCRenderable.h"
#include "GCCamera.h"

class GRAPHICSCORE_EXPORT GCScene : public GCRenderable, public XCameraCanvasController
  {
  S_ENTITY(GCScene, GCRenderable, 0);

public:
  GCScene();

  GCCameraPointer activeCamera;
  ComplexTransformProperty cameraProjection;
  TransformProperty cameraTransform;

  void render(XRenderer *) const;

  virtual CameraInterface *camera();

  GCShadingGroupPointerArray shadingGroups;
  };

class GRAPHICSCORE_EXPORT GCManipulatableScene : public GCScene
  {
  S_ENTITY(GCManipulatableScene, GCScene, 0);

XProperties:
  XROProperty(GCVisualManipulator *, currentManipulator);

public:
  GCManipulatableScene();

  SPropertyArray manipulators;

  void render(XRenderer *) const;

  void clearManipulators();
  void addAllManipulators();

  virtual UsedFlags mouseEvent(const MouseEvent &);
  virtual UsedFlags wheelEvent(const WheelEvent &);
  };

#endif // GCSCENE_H
