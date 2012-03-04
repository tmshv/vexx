#include "MeshCore.h"
#include "styperegistry.h"

#include "MCCube.h"
#include "MCPolyhedron.h"

void initiateMeshCore()
  {
  STypeRegistry::addType(MCPolyhedronProperty::staticTypeInformation());
  STypeRegistry::addType(MCCube::staticTypeInformation());
  }
