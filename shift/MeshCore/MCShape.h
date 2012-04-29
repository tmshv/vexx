#ifndef MCSHAPE_H
#define MCSHAPE_H

#include "3D/GCRenderable.h"
#include "MCGeometry.h"

class MCShape : public GCRenderable
  {
  S_ENTITY(MCShape, GCRenderable, 0)

public:
  MCShape();

  MCGeometry geometry;

  void render(XRenderer *) const;

  virtual void intersect(const XLine& line, Selector *) const;
  virtual void intersect(const XFrustum& frus, Selector *) const;
  };

S_PROPERTY_INTERFACE(MCShape)

#endif // MCSHAPE_H
