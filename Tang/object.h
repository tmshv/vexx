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

class Area : public Object
  {
  S_ENTITY(Area, Object, 0)

public:
  Area();
  };

#endif // OBJECTID_H
