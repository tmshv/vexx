#include "sappdatabase.h"
#include "styperegistry.h"

S_IMPLEMENT_PROPERTY(SAppDatabase)

const SPropertyInformation *SAppDatabase::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<SAppDatabase>("SAppDatabase");
  info->add(&SAppDatabase::document, "document");
  info->add(&SAppDatabase::settings, "settings");
  return info;
  }

SAppDatabase::SAppDatabase() : SDatabase()
  {
  initiateInheritedDatabaseType(staticTypeInformation());
  }
