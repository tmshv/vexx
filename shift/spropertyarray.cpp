#include "spropertyarray.h"
#include "styperegistry.h"

S_IMPLEMENT_PROPERTY(SPropertyArray)

void SPropertyArray::createTypeInformation(SPropertyInformation *info, const SPropertyInformationCreateData &data)
  {
  if(data.registerInterfaces)
    {
    XInterface<SPropertyArray>* api = info->apiInterface<SPropertyArray>();

    XInterfaceBase::Function fn = XScript::MethodToInCa<SPropertyArray, SProperty *(const SPropertyInformation *, const QString &), &SPropertyArray::add>::Call;
    api->addFunction("add", fn);
    }
  }

SPropertyArray::SPropertyArray()
  {
  }

SPropertyArray::~SPropertyArray()
  {
  }

void SPropertyArray::remove(SProperty *prop)
  {
  SPropertyContainer::removeProperty(prop);
  }
