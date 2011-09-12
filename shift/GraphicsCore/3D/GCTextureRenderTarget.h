#ifndef GCRENDERTOTEXTURE_H
#define GCRENDERTOTEXTURE_H

#include "GCGlobal.h"
#include "GCRenderTarget.h"

class GRAPHICSCORE_EXPORT GCTextureRenderTarget : public GCRenderTarget
  {
  S_ENTITY(GCTextureRenderTarget, GCRenderTarget, 0)

public:
  GCTextureRenderTarget();
  };

#endif // GCRENDERTOTEXTURE_H
