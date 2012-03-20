#ifndef GCRENDERABLE_H
#define GCRENDERABLE_H

#include "GCGlobal.h"
#include "sentity.h"
#include "sbasepointerproperties.h"

class XRenderer;

class GRAPHICSCORE_EXPORT GCRenderable : public SEntity
  {
  S_ABSTRACT_ENTITY(GCRenderable, SEntity, 0)

public:
  GCRenderable();

  virtual void render(XRenderer *) const;
  };

S_PROPERTY_INTERFACE(GCRenderable)

S_TYPED_POINTER_TYPE(GCRenderablePointer, GCRenderable)

#endif // GCRENDERABLE_H
