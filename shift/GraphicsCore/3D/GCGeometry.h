#ifndef GCGEOMETRY_H
#define GCGEOMETRY_H

#include "GCGlobal.h"
#include "spropertycontainer.h"
#include "spropertyarray.h"
#include "sarrayproperty.h"
#include "sdatabase.h"
#include "XGeometry.h"
#include "sbasepointerproperties.h"

class GRAPHICSCORE_EXPORT GCPolygonArray : public SPropertyContainer
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

class GRAPHICSCORE_EXPORT GCGeometry : public SPropertyContainer
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

DEFINE_POD_PROPERTY(GRAPHICSCORE_EXPORT, GCRuntimeGeometry, XGeometry, XGeometry())

S_TYPED_POINTER_TYPE(GCGeometryPointer, GCGeometry)

S_TYPED_POINTER_ARRAY_TYPE(GCGeometryPointerArray, GCGeometryPointer)

#endif // GCGEOMETRY_H
