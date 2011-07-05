#ifndef GCRENDERTARGET_H
#define GCRENDERTARGET_H

#include "GCGlobal.h"
#include "sentity.h"
#include "GCRenderable.h"

class GRAPHICSCORE_EXPORT GCRenderTarget : public SEntity
  {
  S_ENTITY(GCRenderTarget, SEntity, 0)

public:
  GCRenderTarget();

  GCRenderablePointer source;

  void render(XRenderer *);
  };

#endif // GCRENDERTARGET_H
