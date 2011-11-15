#include "sappdatabase.h"
#include "styperegistry.h"
#include "sadocument.h"

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

SDocument *SAppDatabase::addDocument(const SPropertyInformation *info)
  {
  SProperty *prop = documents.addChild(info, "");
  if(prop)
    {
    SDocument *doc = prop->castTo<SDocument>();
    xAssert(doc);
    return doc;
    }

  return 0;
  }
