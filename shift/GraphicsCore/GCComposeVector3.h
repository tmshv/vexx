#ifndef GCCOMPOSEVECTOR3_H
#define GCCOMPOSEVECTOR3_H

#include "GCGlobal.h"
#include "sentity.h"
#include "sbaseproperties.h"

class GRAPHICSCORE_EXPORT GCComposeVector3 : public SEntity
  {
  S_ENTITY(GCComposeVector3, SEntity, 0)

public:
  GCComposeVector3();

  FloatProperty xIn;
  FloatProperty yIn;
  FloatProperty zIn;

  Vector3DProperty vectorOut;
  };

S_PROPERTY_INTERFACE(GCComposeVector3)

#endif // GCCOMPOSEVECTOR3_H
