#ifndef GCDISTANCEMANIPULATOR_H
#define GCDISTANCEMANIPULATOR_H

#include "GCManipulator.h"

class GCDistanceManipulator : public GCLinearDragManipulator
  {
  S_PROPERTY_CONTAINER(GCDistanceManipulator, GCLinearDragManipulator, 0);

public:
  GCDistanceManipulator();

  void onDrag(const QPoint &oldWidgetSpacePoint,
              const QPoint &widgetSpacePoint,
              const XVector3D &cameraPosition,
              const XVector3D &oldClickDirection,
              const XVector3D &clickDirection);
  };

#endif // GCDISTANCEMANIPULATOR_H
