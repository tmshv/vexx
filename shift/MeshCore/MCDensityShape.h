#ifndef MCDENSITYSHAPE_H
#define MCDENSITYSHAPE_H

#include "MeshCore.h"
#include "sentity.h"
#include "MCGeometry.h"

class MESHCORE_EXPORT MCDensityShape : public SEntity
  {
  S_ENTITY(MCDensityShape, SEntity, 0)

public:
  MCDensityShape();
  ~MCDensityShape();

  MCGeometry geometry;

  virtual float boundingRadius() const { return 1.0f; }
  virtual float minimumSurfaceAngle() const { return 30.0f; }
  virtual float maximumApproximateTriangleSize() const { return 0.1f; }
  virtual float evaluate(const XVector3D &fn) const;

private:
  static void computeGeometry(const SPropertyInstanceInformation*, MCDensityShape* cube);
  };

S_PROPERTY_INTERFACE(MCDensityShape);

#endif // MCDENSITYSHAPE_H
