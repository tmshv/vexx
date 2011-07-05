#ifndef GCRENDERABLE_H
#define GCRENDERABLE_H

#include "GCGlobal.h"
#include "sentity.h"
#include "sbaseproperties.h"

class XRenderer;

class GRAPHICSCORE_EXPORT GCRenderable : public SEntity
  {
  S_ABSTRACT_PROPERTY(GCRenderable, SEntity, 0)

public:
  GCRenderable();

  void render(XRenderer *) const;
  };

S_TYPED_POINTER_TYPE(GCRenderablePointer, GCRenderable)

#endif // GCRENDERABLE_H
