#include "spropertyarray.h"
#include "styperegistry.h"
#include "spropertyinformationhelpers.h"

S_IMPLEMENT_PROPERTY(SPropertyArray, Shift)

void SPropertyArray::createTypeInformation(SPropertyInformationTyped<SPropertyArray> *info,
                                           const SPropertyInformationCreateData &data)
  {
  if(data.registerInterfaces)
    {
    auto* api = info->apiInterface();

    XInterfaceBase::Function fn = XScript::MethodToInCa<SPropertyArray, SProperty *(const SPropertyInformation *, const QString &), &SPropertyArray::add>::Call;
    api->addFunction("add", fn);
    }
  }

void SPropertyArray::remove(SProperty *prop)
  {
  SPropertyContainer::removeProperty(prop);
  }
