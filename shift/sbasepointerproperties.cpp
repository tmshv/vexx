#include "sbasepointerproperties.h"

S_IMPLEMENT_PROPERTY(Pointer)

SPropertyInformation *Pointer::createTypeInformation()
  {
  return SPropertyInformation::create<Pointer>("Pointer");
  }

void Pointer::setPointed(const SProperty *prop)
  {
  if(hasInput())
    {
    input()->disconnect(this);
    }

  if(prop)
    {
    prop->connect(this);
    }
  }
