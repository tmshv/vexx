#include "sappdatabase.h"

S_ENTITY_DEFINITION(SAppDatabase, SDatabase)
  S_PROPERTY_ENTITY_CHILD_DEFINITION(SEntity, document)
S_ENTITY_END_DEFINITION(SAppDatabase, SDatabase)

SAppDatabase::SAppDatabase() : SDatabase()
  {
  initiateInheritedDatabaseType(staticTypeInformation());
  }
