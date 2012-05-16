#include "spropertygroup.h"
#include "styperegistry.h"

SPropertyGroup::SPropertyGroup() : _added(false)
  {
  }

void SPropertyGroup::registerPropertyInformation(const SPropertyInformation *i)
  {
  _containedTypes << i;
  STypeRegistry::onTypeAdded(this, i);
  }
