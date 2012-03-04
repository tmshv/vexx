#ifndef MCCUBE_H
#define MCCUBE_H

#include "MeshCore.h"
#include "sentity.h"
#include "3D/GCGeometry.h"

class MESHCORE_EXPORT MCCube : public SEntity
  {
  S_ENTITY(MCCube, SEntity, 0)

public:
  MCCube();
  ~MCCube();

  GCGeometry geometry;

private:
  static void computeGeometry(const SPropertyInstanceInformation*, MCCube* cube);

  class Impl;
  Impl* _impl;
  };

#endif // MCCUBE_H
