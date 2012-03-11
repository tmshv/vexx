#ifndef COMPONENT_H
#define COMPONENT_H

#include "sentity.h"

class Component : public SEntity
  {
  S_ENTITY(Component, SEntity, 0)

public:
  Component();
  };

#endif // COMPONENT_H
