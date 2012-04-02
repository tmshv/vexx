#ifndef MCSPHERE_H
#define MCSPHERE_H

#include "MCDensityShape.h"
#include "3D/Manipulators/GCManipulator.h"

class MCSphere : public MCDensityShape, public GCManipulatable
  {
  S_ENTITY(MCSphere, MCDensityShape, 0)

public:
  MCSphere();

  FloatProperty radius;

  virtual float boundingRadius() const { return radius()+0.5f; }
  virtual float minimumSurfaceAngle() const { return 30.0f; }
  virtual float maximumApproximateTriangleSize() const { return 0.75f*radius(); }
  virtual float evaluate(const XVector3D &p) const;

private:
  virtual void addManipulators(SPropertyArray *, const GCTransform *tr=0);
  };

S_PROPERTY_INTERFACE(MCSphere);

#endif // MCSPHERE_H
