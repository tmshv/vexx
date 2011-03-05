#include "spropertyarray.h"
#include "sdatabase.h"

S_PROPERTY_CONTAINER_EMPTY_DEFINITION(SPropertyArray, SPropertyContainer, saveContainer, loadContainer)

SPropertyArray::SPropertyArray()
  {
  }

SPropertyArray::~SPropertyArray()
  {
  }

SProperty *SPropertyArray::at(xsize i)
  {
  SProperty *c = firstChild();
  while(c && i)
    {
    --i;
    c = c->nextSibling();
    }

  return c;
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
