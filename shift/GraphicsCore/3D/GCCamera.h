#ifndef GCCAMERA_H
#define GCCAMERA_H

#include "GCGlobal.h"
#include "GCTransform.h"

class GRAPHICSCORE_EXPORT GCCamera : public GCTransform
  {
  S_ENTITY(GCCamera, GCTransform, 0)

public:
  GCCamera();
  };

#endif // GCCAMERA_H
