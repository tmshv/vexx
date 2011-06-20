#include "spropertyarray.h"
#include "sdatabase.h"

S_IMPLEMENT_PROPERTY(SPropertyArray)

const SPropertyInformation *SPropertyArray::createTypeInformation()
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
