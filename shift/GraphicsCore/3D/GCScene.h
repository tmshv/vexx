#ifndef GCSCENE_H
#define GCSCENE_H

#include "GCShadingGroup.h"
#include "GCRenderable.h"
#include "GCCamera.h"
#include "XFrustum.h"

class GCViewableTransform;

class GRAPHICSCORE_EXPORT GCScene : public GCRenderArray, public XCameraCanvasController
  {
  S_ENTITY(GCScene, GCRenderArray, 0);

public:
  GCScene();

  GCCameraPointer activeCamera;
  ComplexTransformProperty cameraProjection;
  TransformProperty cameraTransform;

  void render(XRenderer *) const;

  void setCamera(GCViewableTransform *e);
  virtual CameraInterface *camera();
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

  struct Hit
    {
    XVector3D position;
    XVector3D normal;
    GCRenderable *object;
    };

  XShader _boundsShader;
  XGeometry _bounds;
  };

S_PROPERTY_INTERFACE(GCManipulatableScene)

#endif // GCSCENE_H
