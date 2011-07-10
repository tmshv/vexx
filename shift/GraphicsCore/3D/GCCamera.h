#ifndef GCCAMERA_H
#define GCCAMERA_H

#include "GCGlobal.h"
#include "GCTransform.h"
#include "sbaseproperties.h"

class XRenderer;

class GRAPHICSCORE_EXPORT GCCamera : public GCTransform
  {
  S_ENTITY(GCCamera, GCTransform, 0)

public:
  GCCamera();

  ComplexTransformProperty projection;

  X_ALIGNED_OPERATOR_NEW
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
