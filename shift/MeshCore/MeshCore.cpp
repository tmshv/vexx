#include "MeshCore.h"
#include "styperegistry.h"

#include "MCGeometry.h"
#include "MCCuboid.h"
#include "MCPolyhedron.h"

void initiateMeshCore()
  {
  STypeRegistry::addType(MCPolyhedronProperty::staticTypeInformation());
  STypeRegistry::addType(MCGeometry::staticTypeInformation());

  STypeRegistry::addType(MCCuboid::staticTypeInformation());
  }
