#ifndef GCCAMERA_H
#define GCCAMERA_H

#include "GCTransform.h"

class GCCamera : public GCTransform
  {
  S_ENTITY(GCCamera, GCTransform)

public:
  GCCamera3D();
  };

#endif // GCCAMERA_H
