#include "spropertyarray.h"
#include "styperegistry.h"

S_IMPLEMENT_PROPERTY(SPropertyArray)

void SPropertyArray::createTypeInformation(SPropertyInformation *, const SPropertyInformationCreateData &)
  {
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
