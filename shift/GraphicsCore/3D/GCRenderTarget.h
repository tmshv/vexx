#ifndef GCRENDERTARGET_H
#define GCRENDERTARGET_H

#include "GCGlobal.h"
#include "sentity.h"

class GRAPHICSCORE_EXPORT GCRenderTarget : public SEntity
  {
  S_ENTITY(GCRenderTarget, SEntity, 0)

public:
  GCRenderTarget();

  void render();
  };

#endif // GCRENDERTARGET_H
