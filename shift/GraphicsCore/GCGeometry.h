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
  void setVertexSize(xuint32 size);

private:
  SUIntArrayProperty _data;
  UnsignedIntProperty _vertexSize;
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
    polygons.setVertexSize(polygons.vertexSize() + 1);
    }

  void removeAttribute(const QString &name)
    {
    SProperty *prop = attributes.findChild(name);
    if(prop)
      {
      SBlock b(database());
      attributes.remove(prop);
      polygons.setVertexSize(polygons.vertexSize() - 1);
      }
    }

  GCGeometry();

private:
  SPropertyArray attributes;
  };

#endif // GCGEOMETRY_H
