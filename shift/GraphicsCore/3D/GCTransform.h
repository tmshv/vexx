#ifndef GCTRANSFORM_H
#define GCTRANSFORM_H

#include "GCGlobal.h"
#include "GCRenderable.h"
#include "XTransform.h"
#include "sbaseproperties.h"
#include "3D/Manipulators/GCManipulator.h"
#include "GCBaseProperties.h"

class GRAPHICSCORE_EXPORT GCTransform : public GCRenderArray, public GCManipulatable
  {
  S_ENTITY(GCTransform, GCRenderArray, 0)

public:
  GCTransform();

  TransformProperty transform;

  virtual void render(XRenderer *) const;

  virtual void addManipulators(SPropertyArray *, const GCTransform *tr=0);

  virtual void intersect(const XLine& line, Selector *);
  virtual void intersect(const XFrustum& frus, Selector *);
  };

S_PROPERTY_INTERFACE(GCTransform)

#endif // GCTRANSFORM3D_H
