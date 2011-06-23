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

void GCPolygonArray::addVertexAttribute(xuint32 index, xuint32 count)
  {
  }

void GCPolygonArray::removeVertexAttribute(xuint32 index, xuint32 count)
  {
  }

void GCPolygonArray::addPolygons(const xuint32 *sizes, xuint32 count)
  {
  }


GCGeometry::GCGeometry()
  {
  }

