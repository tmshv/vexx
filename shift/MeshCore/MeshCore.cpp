#include "MeshCore.h"
#include "styperegistry.h"

#include "MCGeometry.h"
#include "MCCuboid.h"
#include "MCDensityShape.h"
#include "MCSphere.h"
#include "MCPolyhedron.h"

void initiateMeshCore()
  {
  STypeRegistry::addType(MCPolyhedronProperty::staticTypeInformation());
  STypeRegistry::addType(MCGeometry::staticTypeInformation());

  STypeRegistry::addType(MCShape::staticTypeInformation());
  STypeRegistry::addType(MCDensityShape::staticTypeInformation());

  STypeRegistry::addType(MCCuboid::staticTypeInformation());
  STypeRegistry::addType(MCSphere::staticTypeInformation());
  }
