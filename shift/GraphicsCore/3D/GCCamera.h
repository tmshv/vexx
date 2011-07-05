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

  virtual void begin(XRenderer *) const;
  virtual void end(XRenderer *) const;

  X_ALIGNED_OPERATOR_NEW
  };

S_TYPED_POINTER_TYPE(GCCameraPointer, GCCamera)

#endif // GCCAMERA_H
