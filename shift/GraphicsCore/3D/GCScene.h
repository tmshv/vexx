#ifndef GCSCENE_H
#define GCSCENE_H

#include "GCShadingGroup.h"
#include "GCRenderable.h"
#include "GCCamera.h"
#include "XFrustum.h"

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

S_PROPERTY_INTERFACE(GCScene)

class GRAPHICSCORE_EXPORT GCManipulatableScene : public GCScene
  {
  S_ENTITY(GCManipulatableScene, GCScene, 0);

XProperties:
  XROProperty(GCVisualManipulator *, currentManipulator);

public:
  GCManipulatableScene();

  SPropertyArray manipulators;
  PointerArray selection;

  void render(XRenderer *) const;

  void clearManipulators();
  void refreshManipulators();

  void beginMouseSelection(const XVector3D &sel);
  void moveMouseSelection(const XVector3D &sel);
  void endMouseSelection(const XVector3D &sel);
  bool isMouseSelecting() const;

  void raySelect(const XVector3D &sel);
  void marqueeSelect(const XFrustum &frus);

  virtual UsedFlags mouseEvent(const MouseEvent &);
  virtual UsedFlags wheelEvent(const WheelEvent &);

private:
  bool _mouseSelecting;
  bool _hasMouseMoved;
  XVector3D _initialRay;
  XVector3D _finalRay;
  };

S_PROPERTY_INTERFACE(GCManipulatableScene)

#endif // GCSCENE_H
