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
    SProperty *prop = at(1);
    if(!prop)
      {
      xAssertFail();
      return 0;
      }

    T *t = prop->castTo<T>();
    return t;
    }

  template <typename T>const T *attributeData() const
    {
    const SProperty *prop = at(1);
    if(!prop)
      {
      xAssertFail();
      return 0;
      }

    const T *t = prop->castTo<T>();
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

DEFINE_POD_PROPERTY(GRAPHICSCORE_EXPORT, GCRuntimeGeometry, XGeometry, XGeometry())

class GRAPHICSCORE_EXPORT GCGeometry : public SPropertyContainer
  {
  S_PROPERTY_CONTAINER(GCGeometry, SPropertyContainer, 0)

public:
  GCGeometry();

  STypedPropertyArray<GCGeometryAttribute> attributes;
  GCRuntimeGeometry runtimeGeometry;

  template <typename T>
  GCGeometryAttribute *addAttribute(const QString &name)
    {
    SBlock b(database());
    GCGeometryAttribute *t = attributes.add();
    xAssert(t);

    t->setName(name);
    t->setType(T::staticTypeInformation());

    return t;
    }

  void removeAttribute(const QString &name)
    {
    SProperty *prop = attributes.findChild(name);
    if(prop)
      {
      attributes.remove(prop);
      }
    }

  GCGeometryAttribute *attribute(const QString &name)
    {
    SProperty *prop = attributes.findChild(name);
    if(prop)
      {
      return prop->castTo<GCGeometryAttribute>();
      }
    return 0;
    }

  const GCGeometryAttribute *attribute(const QString &name) const
    {
    const SProperty *prop = findChild(name);
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

  void appendTo(XGeometry *geo) const;
  };

S_TYPED_POINTER_TYPE(GCGeometryPointer, GCGeometry)

S_TYPED_POINTER_ARRAY_TYPE(GCGeometryPointerArray, GCGeometryPointer)

#endif // GCGEOMETRY_H
