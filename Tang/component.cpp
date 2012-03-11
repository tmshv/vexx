#include "component.h"

S_IMPLEMENT_PROPERTY(Component);

SPropertyInformation *Component::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<Component>("Component");

  return info;
  }

Component::Component()
  {
  }
