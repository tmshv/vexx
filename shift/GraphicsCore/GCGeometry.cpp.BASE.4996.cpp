#include "GCGeometry.h"
#include "styperegistry.h"

S_IMPLEMENT_PROPERTY(GCGeometry)

const SPropertyInformation *GCGeometry::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCGeometry>("GCGeometry");
  info->add(&GCGeometry::polygons, "polygons");
  info->add(&GCGeometry::attributes, "attributes");
  return info;
  }

S_IMPLEMENT_PROPERTY(GCPolygonArray)

const SPropertyInformation *GCPolygonArray::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCPolygonArray>("GCPolygonArray");
  info->add(&GCPolygonArray::_data, "_data");
  info->add(&GCPolygonArray::_vertexSize, "_vertexSize");
  return info;
  }

GCPolygonArray::GCPolygonArray()
  {
  }

GCGeometry::GCGeometry()
  {
  }

void setVertexSize(xuint32 size)
  {
  xAssertFail();
  }
