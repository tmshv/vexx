#include "MeshCore.h"
#include "styperegistry.h"
#include "spropertygroup.h"

namespace MeshCore
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
