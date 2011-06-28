#include "GraphicsCore.h"
#include "GCImage.h"
#include "GCQImage.h"
#include "3D/GCGeometry.h"
#include "styperegistry.h"

void initiateGraphicsCore(SDatabase *db)
  {
  STypeRegistry::addType(GCImage::staticTypeInformation());
  STypeRegistry::addType(GCQImage::staticTypeInformation());
  STypeRegistry::addType(GCPolygonArray::staticTypeInformation());
  STypeRegistry::addType(GCGeometry::staticTypeInformation());
  }
