#ifndef MCCUBOID_H
#define MCCUBOID_H

#include "MCShape.h"
#include "3D/Manipulators/GCManipulator.h"

class MCCuboid : public MCShape, public GCManipulatable
  {
  S_ENTITY(MCCuboid, MCShape, 0)

public:
  MCCuboid();

  FloatProperty width;
  FloatProperty height;
  FloatProperty depth;

private:
  virtual void addManipulators(SPropertyArray *, const GCTransform *tr=0);
  static void computeGeometry(const SPropertyInstanceInformation*, MCCuboid* cube);
  };

S_PROPERTY_INTERFACE(MCCuboid);

#endif // MCSPHERE_H
