#include "MCGlobal.h"
#include "styperegistry.h"
#include "spropertygroup.h"

namespace MathsCore
{
void initiate()
  {
  STypeRegistry::addPropertyGroup(propertyGroup());
  }

SPropertyGroup &propertyGroup()
  {
  static SPropertyGroup grp;
  return grp;
  }
}
