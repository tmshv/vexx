#ifndef GCDISTANCEMANIPULATOR_H
#define GCDISTANCEMANIPULATOR_H

#include "GCManipulator.h"
#include "sbaseproperties.h"
#include "sbasepointerproperties.h"

class GCDistanceManipulator : public GCLinearDragManipulator
  {
  S_PROPERTY_CONTAINER(GCDistanceManipulator, GCLinearDragManipulator, 0);

public:
  GCDistanceManipulator();

  FloatProperty distance;
  FloatProperty scaleFactor;

  // displacement moved from click point in local space
  Vector3DProperty absoluteDisplacement;

  void addDriven(FloatProperty *in);

  void onDrag(const MouseMoveEvent &);

private:
  QVector <FloatProperty *> _driven;
  };

#endif // GCDISTANCEMANIPULATOR_H
