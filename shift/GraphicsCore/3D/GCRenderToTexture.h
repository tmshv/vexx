#ifndef GCRENDERTOTEXTURE_H
#define GCRENDERTOTEXTURE_H

#include "GCRenderTarget.h"

class GCRenderToTexture : public GCRenderTarget
  {
  S_ENTITY(GCRenderToTexture, GCRenderTarget, 0)

public:
    GCRenderToTexture();
  };

#endif // GCRENDERTOTEXTURE_H
