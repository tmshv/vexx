#ifndef GCTEXTURE_H
#define GCTEXTURE_H

#include "GCGlobal.h"
#include "sentity.h"
#include "sbasepointerproperties.h"
#include "XTexture.h"

class GRAPHICSCORE_EXPORT GCTexture : public SEntity
  {
  S_ENTITY(GCTexture, SEntity, 0)

XRORefProperty(XTexture, texture);

public:
  GCTexture();
  };


S_TYPED_POINTER_TYPE(GCTexturePointer, GCTexture);
#endif // GCTEXTURE_H
