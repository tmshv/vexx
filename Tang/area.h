#ifndef AREA_H
#define AREA_H

#include "sentity.h"
#include "3D/GCScene.h"

class Area : public GCManipulatableScene
  {
  S_ENTITY(Area, GCManipulatableScene, 0)

public:
  Area();

  PointerArray shaders;
  };

S_PROPERTY_INTERFACE(Area)

#endif // AREA_H
