#include "GCGeometry.h"
#include "styperegistry.h"

S_IMPLEMENT_PROPERTY(GCGeometry)

SPropertyInformation *GCGeometry::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCGeometry>("GCGeometry");
  info->add(&GCGeometry::polygons, "polygons");
  info->add(&GCGeometry::attributes, "attributes");
  return info;
  }

S_IMPLEMENT_PROPERTY(GCPolygonArray)

SPropertyInformation *GCPolygonArray::createTypeInformation()
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

  SUIntArrayProperty::EigenArray newData;

  // old data size plus an extra count indices per polygon
  newData.resize(1, (numVertices * _vertexSize()) + _polygonCount());

  xuint32* newPolygonPtr = newData.data();

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
      memcpy(newPolygonPtr, oldPolygonPtr, sizeof(SUIntArrayProperty::ElementType)*oldVertexSize*polygonSize);
      newPolygonPtr += oldVertexSize * polygonSize;
      oldPolygonPtr += oldVertexSize * polygonSize;

      memset(newPolygonPtr, 0, sizeof(SUIntArrayProperty::ElementType)*count*polygonSize);
      newPolygonPtr += count * polygonSize;
      }
    }

  _data.setData(newData);
  }

void GCPolygonArray::removeVertexAttribute(xuint32 index, xuint32 count)
  {
  const SUIntArrayProperty::EigenArray oldData = _data.data();
  const xuint32* oldPolygonPtr = oldData.data();

  xuint32 oldVertexSize = _vertexSize();
  _vertexSize = oldVertexSize + count;

  xuint32 numVertices = ((oldData.size() - _polygonCount()) / (_polygonCount() * oldVertexSize));
  xAssert(((oldData.size() - _polygonCount()) % (_polygonCount() * oldVertexSize)) == 0);

  SUIntArrayProperty::EigenArray newData;

  // old data size plus an extra count indices per polygon
  newData.resize(1, (numVertices * _vertexSize()) + _polygonCount());

  xuint32* newPolygonPtr = newData.data();

  for(xuint32 poly=0, polys=_polygonCount(); poly<polys; ++poly)
    {
    const xuint32 polygonSize = *oldPolygonPtr;
    *newPolygonPtr = polygonSize;

    oldPolygonPtr++;
    newPolygonPtr++;

    for(xuint32 vert=0; vert<polygonSize; ++vert)
      {
      memcpy(newPolygonPtr, oldPolygonPtr, sizeof(SUIntArrayProperty::ElementType)*index);
      newPolygonPtr += index * polygonSize;
      oldPolygonPtr += index * polygonSize;

      oldPolygonPtr += count * polygonSize;

      xuint32 leftToCopy = oldVertexSize - count - index;

      memcpy(newPolygonPtr, oldPolygonPtr, sizeof(SUIntArrayProperty::ElementType)*leftToCopy*polygonSize);
      newPolygonPtr += leftToCopy * polygonSize;
      oldPolygonPtr += leftToCopy * polygonSize;
      }
    }

  _data.setData(newData);
  }

void GCPolygonArray::addPolygons(const xuint32 *sizes, xuint32 count)
  {
  }


GCGeometry::GCGeometry()
  {
  }

