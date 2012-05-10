#ifndef MCSIMPLE_H
#define MCSIMPLE_H

#include "sentity.h"
#include "sbaseproperties.h"

class MCSimple : public SEntity
  {
  S_ENTITY(MCSimple, SEntity, 0)

public:
  MCSimple();

  FloatProperty inputA;
  FloatProperty inputB;

  FloatProperty output;
  };

S_PROPERTY_INTERFACE(MCSimple)

#endif // MCSIMPLE_H
