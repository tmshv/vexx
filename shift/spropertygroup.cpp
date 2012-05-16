#include "spropertygroup.h"

SPropertyGroup::SPropertyGroup() : _added(false)
  {
  }

void SPropertyGroup::registerPropertyInformation(const SPropertyInformation *i)
  {
  xAssert(!_added);
  _containedTypes << i;
  }

SPropertyGroup &shiftPropertyGroup()
  {
  static SPropertyGroup grp;
  return grp;
  }
