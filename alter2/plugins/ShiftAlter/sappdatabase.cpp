#include "sappdatabase.h"
#include "spropertyinformationhelpers.h"
#include "styperegistry.h"
#include "sadocument.h"

S_IMPLEMENT_PROPERTY(SAppDatabase, ShiftAlter)

void SAppDatabase::createTypeInformation(SPropertyInformationTyped<SAppDatabase> *info,
                                         const SPropertyInformationCreateData &data)
  {
  if(data.registerAttributes)
    {
    auto childInst = info->child(&SAppDatabase::children);
    auto childrenExt = info->extendContainedProperty(childInst);

    childrenExt->add(&SAppDatabase::settings, "settings");
    }

  if(data.registerInterfaces)
    {
    auto api = info->apiInterface();

    api->addMethod<SDocument *(const SPropertyInformation *), &SAppDatabase::addDocument>("addDocument");
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
