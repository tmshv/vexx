#include "spropertyarray.h"
#include "sdatabase.h"

S_PROPERTY_CONTAINER_EMPTY_DEFINITION(SPropertyArray)

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
