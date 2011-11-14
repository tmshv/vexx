#include "sappdatabase.h"
#include "styperegistry.h"

S_IMPLEMENT_PROPERTY(SAppDatabase)

SPropertyInformation *SAppDatabase::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<SAppDatabase>("SAppDatabase");
  info->add(&SAppDatabase::documents, "documents");
  info->add(&SAppDatabase::settings, "settings");
  return info;
  }

SAppDatabase::SAppDatabase() : SDatabase()
  {
  initiateInheritedDatabaseType(staticTypeInformation());
  }
