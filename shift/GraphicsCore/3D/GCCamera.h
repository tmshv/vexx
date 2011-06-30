#ifndef GCCAMERA_H
#define GCCAMERA_H

#include "GCTransform.h"

class GCCamera : public GCTransform
  {
  S_ENTITY(GCCamera, GCTransform, 0)

public:
  GCCamera();
  };

#endif // GCCAMERA_H
