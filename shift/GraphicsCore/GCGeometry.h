#ifndef GCGEOMETRY_H
#define GCGEOMETRY_H

#include "GCGlobal.h"
#include "spropertycontainer.h"
#include "spropertyarray.h"
#include "sarrayproperty.h"
#include "sdatabase.h"

class GCPolygonArray : public SPropertyContainer
  {
  S_PROPERTY_CONTAINER(GCPolygonArray, SPropertyContainer, 0)

public:
  GCPolygonArray();

  xuint32 vertexSize() const { return _vertexSize(); }
  void addVertexAttribute(xuint32 count=1);
  void removeVertexAttribute(xuint32 index, xuint32 count=1);

  void addPolygon(xuint32 size) { addPolygons(&size, 1); }
  void addPolygons(const XVector<xuint32> &sizes) { addPolygons(&sizes.front(), sizes.size()); }
  void addPolygons(const xuint32 *sizes, xuint32 count);

private:
  SUIntArrayProperty _data;
  UnsignedIntProperty _vertexSize;
  UnsignedIntProperty _polygonCount;
  };

class GCGeometry : public SPropertyContainer
  {
  S_PROPERTY_CONTAINER(GCGeometry, SPropertyContainer, 0)

public:
  GCPolygonArray polygons;

  template <typename T>
  T *addAttribute(const QString &name)
    {
    SBlock b(database());
    T* result = attributes.add<T>();
    polygons.addVertexAttribute();
    }

  void removeAttribute(const QString &name)
    {
    SProperty *prop = attributes.findChild(name);
    if(prop)
      {
      SBlock b(database());
      polygons.removeVertexAttribute(prop->index());
      attributes.remove(prop);
      }
    }

  GCGeometry();

private:
  SPropertyArray attributes;
  };

#endif // GCGEOMETRY_H
