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

  virtual void render(XRenderer *) const = 0;
  };

S_PROPERTY_INTERFACE(GCRenderable)
S_TYPED_POINTER_TYPE(GCRenderablePointer, GCRenderable)
S_TYPED_POINTER_ARRAY_TYPE(GCRenderablePointerArray, GCRenderablePointer)

class GRAPHICSCORE_EXPORT GCRenderArray : public GCRenderable
  {
  S_ABSTRACT_ENTITY(GCRenderArray, GCRenderable, 0)

public:
  GCRenderArray();

  GCRenderablePointerArray renderGroup;

  virtual void render(XRenderer *) const;
  };

S_PROPERTY_INTERFACE(GCRenderArray)

#endif // GCRENDERABLE_H
