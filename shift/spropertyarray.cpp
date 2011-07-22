#include "spropertyarray.h"
#include "styperegistry.h"

S_IMPLEMENT_PROPERTY(SPropertyArray)

SPropertyInformation *SPropertyArray::createTypeInformation()
  {
  return SPropertyInformation::create<SPropertyArray>("SPropertyArray");
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

void SPropertyArray::clear()
  {
  while(firstChild())
    {
    remove(firstChild());
    }
  }
