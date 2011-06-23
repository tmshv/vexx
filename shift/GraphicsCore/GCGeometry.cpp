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
  info->add(&GCPolygonArray::_polygonCount, "_polygonCount");
  return info;
  }

GCPolygonArray::GCPolygonArray()
  {
  }

void GCPolygonArray::addVertexAttribute(xuint32 count)
  {
  const SUIntArrayProperty::EigenArray oldData = _data.data();
  const xuint32* oldPolygonPtr = oldData.data();

  xuint32 oldVertexSize = _vertexSize();
  _vertexSize = oldVertexSize + count;

  xuint32 numVertices = ((oldData.size() - _polygonCount()) / (_polygonCount() * oldVertexSize));
  xAssert(((oldData.size() - _polygonCount()) % (_polygonCount() * oldVertexSize)) == 0);

  // old data size plus an extra count indices per polygon
  _data.resize((numVertices * _vertexSize()) + _polygonCount());
  xuint32* newPolygonPtr = _data.lockData(false).data();

  // so the memory format of the indexes is
  // Data = | PolygonSize | PolygonData |
  //
  // PolygonData = | AttributeAIndices | AttributeBIndices | AttributeCIndices | etc |
  //


  for(xuint32 poly=0, polys=_polygonCount(); poly<polys; ++poly)
    {
    const xuint32 polygonSize = *oldPolygonPtr;
    *newPolygonPtr = polygonSize;

    oldPolygonPtr++;
    newPolygonPtr++;

    for(xuint32 vert=0; vert<polygonSize; ++vert)
      {
      memcpy(newPolygonPtr, oldPolygonPtr, sizeof(SUIntArrayProperty::ArrayElementType)*oldVertexSize);
      newPolygonPtr += oldVertexSize;
      oldPolygonPtr += oldVertexSize;

      memset(newPolygonPtr, 0, sizeof(SUIntArrayProperty::ArrayElementType)*count);
      newPolygonPtr += count;
      }
    }

  _data.unlockData();
  }

void GCPolygonArray::removeVertexAttribute(xuint32 index, xuint32 count)
  {
  const SUIntArrayProperty::EigenArray oldData = _data.data();
  const xuint32* oldPolygonPtr = oldData.data();

  xuint32 oldVertexSize = _vertexSize();
  _vertexSize = oldVertexSize + count;

  xuint32 numVertices = ((oldData.size() - _polygonCount()) / (_polygonCount() * oldVertexSize));
  xAssert(((oldData.size() - _polygonCount()) % (_polygonCount() * oldVertexSize)) == 0);

  // old data size plus an extra count indices per polygon
  _data.resize((numVertices * _vertexSize()) + _polygonCount());
  xuint32* newPolygonPtr = _data.lockData(false).data();

  for(xuint32 poly=0, polys=_polygonCount(); poly<polys; ++poly)
    {
    const xuint32 polygonSize = *oldPolygonPtr;
    *newPolygonPtr = polygonSize;

    oldPolygonPtr++;
    newPolygonPtr++;

    for(xuint32 vert=0; vert<polygonSize; ++vert)
      {
#warning implement
      }
    }

  _data.unlockData();
  }

void GCPolygonArray::addPolygons(const xuint32 *sizes, xuint32 count)
  {
  }


GCGeometry::GCGeometry()
  {
  }

