#ifndef OBJECTID_H
#define OBJECTID_H

#include "XGlobal"
#include "QByteArray"
#include "sentity.h"

class Object : public SEntity
  {
  S_ENTITY(Object, SEntity, 0)

public:
  Object();
  };

S_PROPERTY_INTERFACE(Object)

#endif // OBJECTID_H
