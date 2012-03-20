#ifndef GCCAMERA_H
#define GCCAMERA_H

#include "GCGlobal.h"
#include "GCTransform.h"
#include "sbasepointerproperties.h"
#include "XCameraCanvasController.h"

class XRenderer;

class GRAPHICSCORE_EXPORT GCViewableTransform : public GCTransform, public XCameraCanvasController::CameraInterface
  {
  S_ABSTRACT_ENTITY(GCViewableTransform, GCTransform, 0)

XProperties:
  XProperty(bool, rotateEnabled, setRotateEnabled);

public:
  GCViewableTransform();

  Vector3DProperty upVector;
  FloatProperty focalDistance;

  ComplexTransformProperty projection;
  ComplexTransformProperty inverseProjection;

  UnsignedIntProperty viewportX;
  UnsignedIntProperty viewportY;
  UnsignedIntProperty viewportWidth;
  UnsignedIntProperty viewportHeight;

  TransformProperty viewTransform;

  void setPosition(const XVector3D &point);

  void setFocalPoint(const XVector3D &point);
  XVector3D focalPoint() const;

  void approximatePixelSizeAtDistance(float distance, float &x, float &y) const;
  XTransform getPixelScaleFacingTransform(const XVector3D &worldPosition) const;

  bool unitViewportCoordinates(xuint32 x, xuint32 y, float &xUnit, float &yUnit) const;
  XVector3D worldSpaceFromScreenSpace(xuint32 x, xuint32 y) const;
  virtual XVector3D worldSpaceAtDepthFromScreenSpace(xuint32 x, xuint32 y, float depth) const = 0;

  void zoom(float factor, float x, float y);
  void track(float x, float y);
  void dolly(float x, float y);
  void pan(float x, float y);
  void rotateAboutPoint(const XVector3D &point, float x, float y);
  };

S_TYPED_POINTER_TYPE(GCViewableTransformPointer, GCViewableTransform)
S_PROPERTY_ABSTRACT_INTERFACE(GCViewableTransform)

class GRAPHICSCORE_EXPORT GCCamera : public GCViewableTransform
  {
  S_ABSTRACT_ENTITY(GCCamera, GCViewableTransform, 0)

public:
  GCCamera();
  };

S_PROPERTY_ABSTRACT_INTERFACE(GCCamera)

class GRAPHICSCORE_EXPORT GCPerspectiveCamera : public GCCamera
  {
  S_ENTITY(GCPerspectiveCamera, GCCamera, 0)

public:
  GCPerspectiveCamera();

  virtual XVector3D worldSpaceAtDepthFromScreenSpace(xuint32 x, xuint32 y, float depth) const;

  FloatProperty fieldOfView;

  FloatProperty nearClip;
  FloatProperty farClip;
  };

S_PROPERTY_INTERFACE(GCPerspectiveCamera)

S_TYPED_POINTER_TYPE(GCCameraPointer, GCCamera)

#endif // GCCAMERA_H
