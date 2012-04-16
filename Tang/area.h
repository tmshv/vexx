#ifndef AREA_H
#define AREA_H

#include "sentity.h"
#include "3D/GCRenderable.h"
#include "spropertyarray.h"

class Area : public SEntity
  {
  S_ENTITY(Area, SEntity, 0)

public:
  Area();

  SPropertyArray shaders;
  SPropertyArray shadingGroups;
  };

S_PROPERTY_INTERFACE(Area)

#endif // AREA_H
