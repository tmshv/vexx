#include "sappdatabase.h"

S_ENTITY_DEFINITION(SAppDatabase, SDatabase)
  S_PROPERTY_DEFINITION(SEntity, document)
S_ENTITY_END_DEFINITION(SAppDatabase, SDatabase)

SAppDatabase::SAppDatabase() : SDatabase()
  {
  initiatePropertyFromMetaData(this, staticTypeInformation(), false);
  }
