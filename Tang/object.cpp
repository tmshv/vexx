#include "object.h"
#include "spropertyinformationhelpers.h"
#include "tangmanager.h"

S_IMPLEMENT_PROPERTY(Object, Tang)

void Object::createTypeInformation(SPropertyInformationTyped<Object> *, const SPropertyInformationCreateData &)
  {
  }

Object::Object()
  {
  }
