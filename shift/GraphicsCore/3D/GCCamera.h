#ifndef GCCAMERA_H
#define GCCAMERA_H

#include "GCGlobal.h"
#include "GCTransform.h"
#include "sbaseproperties.h"
#include "XCameraCanvasController.h"

class XRenderer;

class GRAPHICSCORE_EXPORT GCViewableTransform : public GCTransform, public XCameraCanvasController::CameraInterface
  {
  S_ENTITY(GCViewableTransform, GCTransform, 0)

public:
  GCViewableTransform();

  Vector3DProperty upVector;
  FloatProperty focalDistance;
  ComplexTransformProperty projection;

  TransformProperty viewTransform;

  void setPosition(const XVector3D &point);

  void setFocalPoint(const XVector3D &point);
  XVector3D focalPoint() const;

  void zoom(float factor, float x, float y);
  void track(float x, float y);
  void dolly(float x, float y);
  void pan(float x, float y);
  void rotateAboutPoint(const XVector3D &point, float x, float y);
  };

class GRAPHICSCORE_EXPORT GCCamera : public GCViewableTransform
  {
  S_ENTITY(GCCamera, GCViewableTransform, 0)

public:
  GCCamera();
  };

class GRAPHICSCORE_EXPORT GCPerspectiveCamera : public GCCamera
  {
  S_ENTITY(GCPerspectiveCamera, GCCamera, 0)

public:
  GCPerspectiveCamera();

  FloatProperty fieldOfView;

  FloatProperty aspectRatio;

  FloatProperty nearClip;
  FloatProperty farClip;
  };

S_TYPED_POINTER_TYPE(GCCameraPointer, GCCamera)

#endif // GCCAMERA_H
