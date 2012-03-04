#ifndef MCCUBOID_H
#define MCCUBOID_H

#include "MeshCore.h"
#include "sentity.h"
#include "MCGeometry.h"
#include "3D/Manipulators/GCManipulator.h"

class MESHCORE_EXPORT MCCuboid : public SEntity, public GCManipulatable
  {
  S_ENTITY(MCCuboid, SEntity, 0)

public:
  MCCuboid();
  ~MCCuboid();

  MCGeometry geometry;

  FloatProperty width;
  FloatProperty height;
  FloatProperty depth;

  void addManipulators(SPropertyArray *, const GCTransform *tr=0);

private:
  static void computeGeometry(const SPropertyInstanceInformation*, MCCuboid* cube);
  };

#endif // MCCUBOID_H
