#ifndef COMPONENT_H
#define COMPONENT_H

#include "3D/GCRenderable.h"

class Component : public GCRenderArray
  {
  S_ENTITY(Component, GCRenderArray, 0)

public:
  Component();

  void intersect(const XLine &line, Selector *) X_OVERRIDE;
  void intersect(const XFrustum &frus, Selector *) X_OVERRIDE;
  };

S_PROPERTY_INTERFACE(Component)

#endif // COMPONENT_H
