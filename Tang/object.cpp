#include "object.h"

S_IMPLEMENT_PROPERTY(Object)

SPropertyInformation *Object::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<Object>("Object");
  return info;
  }

Object::Object()
  {
  }

S_IMPLEMENT_PROPERTY(Area)

SPropertyInformation *Area::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<Area>("Area");
  return info;
  }

Area::Area()
  {
  }
