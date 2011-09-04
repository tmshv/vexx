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

  FloatProperty scaleFactor;

  void addDriven(SProperty *in);

  void onDrag(const MouseMoveEvent &);
  };

#endif // GCDISTANCEMANIPULATOR_H
