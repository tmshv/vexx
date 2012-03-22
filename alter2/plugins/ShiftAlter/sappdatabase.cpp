#include "sappdatabase.h"
#include "styperegistry.h"
#include "sadocument.h"

S_IMPLEMENT_PROPERTY(SAppDatabase)

void SAppDatabase::createTypeInformation(SPropertyInformation *info, const SPropertyInformationCreateData &data)
  {
  if(data.registerAttributes)
    {
    SPropertyArray::InstanceInformation *childInst = info->child(&SAppDatabase::children);
    SPropertyInformation* childrenExt = info->extendContainedProperty(childInst);

    childrenExt->add(&SAppDatabase::settings, "settings");
    }
  }

SAppDatabase::SAppDatabase() : SDatabase(), _plugin(0)
  {
  initiateInheritedDatabaseType(staticTypeInformation());
  }

SDocument *SAppDatabase::addDocument(const SPropertyInformation *info)
  {
  SProperty *prop = addChild(info, "");
  if(prop)
    {
    SDocument *doc = prop->castTo<SDocument>();

    xAssert(_plugin);
    doc->setPlugin(_plugin);

    xAssert(doc);
    return doc;
    }

  return 0;
  }
