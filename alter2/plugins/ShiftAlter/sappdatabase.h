#ifndef SAPPDATABASE_H
#define SAPPDATABASE_H

#include "saglobal.h"
#include "sdatabase.h"
#include "sentity.h"

class SAppDatabase : public SDatabase
  {
  S_ENTITY(SAppDatabase, SDatabase, 0);

public:
  SEntity document;

  SAppDatabase();
  };

#endif // SAPPDATABASE_H
