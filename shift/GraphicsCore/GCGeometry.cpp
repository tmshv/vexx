#include "GCGeometry.h"

S_PROPERTY_CONTAINER_DEFINITION(GCGeometry)
  S_PROPERTY_DEFINITION(GCPolygonArray, polygons)
  S_PROPERTY_DEFINITION(SPropertyArray, attributes)
S_PROPERTY_CONTAINER_END_DEFINITION(GCGeometry)

S_PROPERTY_CONTAINER_DEFINITION(GCPolygonArray)
  S_PROPERTY_DEFINITION(SUIntArrayProperty, _data)
  S_PROPERTY_DEFINITION(UnsignedIntProperty, _vertexSize, 0)
S_PROPERTY_CONTAINER_END_DEFINITION(GCPolygonArray)

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
