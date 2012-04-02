#ifndef COMPONENT_H
#define COMPONENT_H

#include "sentity.h"

class Component : public SEntity
  {
  S_ENTITY(Component, SEntity, 0)

public:
  Component();
  };

S_PROPERTY_INTERFACE(Component)

#endif // COMPONENT_H
