#include "sappdatabase.h"

S_ENTITY_DEFINITION(SAppDatabase)
  S_PROPERTY_ENTITY_CHILD_DEFINITION(SEntity, document)
  S_PROPERTY_ENTITY_CHILD_DEFINITION(SEntity, settings)
S_ENTITY_END_DEFINITION(SAppDatabase)

SAppDatabase::SAppDatabase() : SDatabase()
  {
  initiateInheritedDatabaseType(staticTypeInformation());
  }
