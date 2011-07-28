#ifndef GCGEOMETRY_H
#define GCGEOMETRY_H

#include "GCGlobal.h"
#include "spropertycontainer.h"
#include "spropertyarray.h"
#include "sarrayproperty.h"
#include "sdatabase.h"
#include "XGeometry.h"
#include "sbasepointerproperties.h"

class GRAPHICSCORE_EXPORT GCGeometryAttribute : public SPropertyContainer
  {
  S_PROPERTY_CONTAINER(GCGeometryAttribute, SPropertyContainer, 0)

public:
  GCGeometryAttribute();

  void setType(const SPropertyInformation *type);

  template <typename T>T *attributeData()
    {
    SProperty *prop = firstChild();
    if(!prop)
      {
      xAssertFail();
      return 0;
      }

    T *t = prop->castTo<T>();
    xAssert(t);

    return t;
    }

  void addPolygon(xuint32 size) { addPolygons(&size, 1); }
  void addPolygons(const XVector<xuint32> &sizes) { addPolygons(&sizes.front(), sizes.size()); }
  void addPolygons(const xuint32 *sizes, xuint32 count);

  void removePolygons(xuint32 index, xuint32 count);

  void clear();

  void setPolygon(xuint32 index, const XVector<xuint32> &indices) { setPolygon(index, &indices.front()); }
  void setPolygon(xuint32 index, const xuint32 *indices);

  SUIntArrayProperty polygons;
  };

class GRAPHICSCORE_EXPORT GCGeometry : public SPropertyContainer
  {
  S_PROPERTY_CONTAINER(GCGeometry, SPropertyContainer, 0)

public:
  GCGeometry();

  void appendTo(XGeometry *geo) const;

  template <typename T>
  GCGeometryAttribute *addAttribute(const QString &name)
    {
    SBlock b(database());
    SProperty *attr = addProperty(GCGeometryAttribute::staticTypeInformation());
    xAssert(attr);

    attr->setName(name);
    GCGeometryAttribute* t = attr->castTo<GCGeometryAttribute>();
    t->setType(T::staticTypeInformation());
    return t;
    }

  void removeAttribute(const QString &name)
    {
    SProperty *prop = findChild(name);
    if(prop)
      {
      removeProperty(prop);
      }
    }

  GCGeometryAttribute *attribute(const QString &name)
    {
    SProperty *prop = findChild(name);
    if(prop)
      {
      return prop->castTo<GCGeometryAttribute>();
      }
    return 0;
    }

  void addPolygon(xuint32 size) { addPolygons(&size, 1); }
  void addPolygons(const XVector<xuint32> &sizes) { addPolygons(&sizes.front(), sizes.size()); }
  void addPolygons(const xuint32 *sizes, xuint32 count);

  void removePolygons(xuint32 index, xuint32 count);

  void clearAttributes();
  };

DEFINE_POD_PROPERTY(GRAPHICSCORE_EXPORT, GCRuntimeGeometry, XGeometry, XGeometry())

S_TYPED_POINTER_TYPE(GCGeometryPointer, GCGeometry)

S_TYPED_POINTER_ARRAY_TYPE(GCGeometryPointerArray, GCGeometryPointer)

#endif // GCGEOMETRY_H
