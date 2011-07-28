#include "GCGeometry.h"
#include "styperegistry.h"

void writeValue(SSaver &s, const XGeometry &t)
  {
  xAssertFail();
  }

void readValue(SLoader &l, XGeometry &t)
  {
  xAssertFail();
  }

IMPLEMENT_POD_PROPERTY(GCRuntimeGeometry, XGeometry)

void GCRuntimeGeometry::assignProperty(const SProperty *f, SProperty *t)
  {
  GCRuntimeGeometry *to = t->uncheckedCastTo<GCRuntimeGeometry>();

  const GCRuntimeGeometry *sProp = f->castTo<GCRuntimeGeometry>();
  if(sProp)
    {
    to->assign(sProp->value());
    return;
    }
  }

S_IMPLEMENT_PROPERTY(GCGeometry)

SPropertyInformation *GCGeometry::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCGeometry>("GCGeometry");
  return info;
  }

S_IMPLEMENT_PROPERTY(GCGeometryAttribute)

SPropertyInformation *GCGeometryAttribute::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCGeometryAttribute>("GCGeometryAttribute");
  info->add(&GCGeometryAttribute::polygons, "polygons");
  return info;
  }

GCGeometryAttribute::GCGeometryAttribute()
  {
  }

void GCGeometryAttribute::setType(const SPropertyInformation *type)
  {
  SBlock b(database());
  if(firstChild())
    {
    removeProperty(firstChild());
    }

  addProperty(type);
  }

void GCGeometryAttribute::addPolygons(const xuint32 *sizes, xuint32 count)
  {
  xuint32 expandBy = 0;
  for(xuint32 i=0; i<s; ++i)
    {
    expandBy += 1 + sizes[i];
    }

  SUIntArrayProperty::EigenArray data = polygons.data();
  xuint32 offset = data.rows();

  for(xuint32 i=0; i<s; ++i)
    {
    data()
    ###
    }

  polygons.setData(data);
  }

void GCGeometryAttribute::removePolygons(xuint32 index, xuint32 count)
  {
  }

void GCGeometryAttribute::setPolygon(xuint32 index, const xuint32 *indices)
  {
  }

GCGeometry::GCGeometry()
  {
  }

void GCGeometry::addPolygons(const xuint32 *sizes, xuint32 count)
  {
  for(GCGeometryAttribute *child=firstChild<GCGeometryAttribute>(); child; child=child->nextSibling<GCGeometryAttribute>())
    {
    child->addPolygons(sizes, count);
    }
  }

void GCGeometry::removePolygons(xuint32 index, xuint32 count)
  {
  for(GCGeometryAttribute *child=firstChild<GCGeometryAttribute>(); child; child=child->nextSibling<GCGeometryAttribute>())
    {
    child->removePolygons(index, count);
    }
  }

void GCGeometry::appendTo(XGeometry *geo) const
  {
  }

void GCGeometry::clearAttributes()
  {
  SBlock b(database());
  while(firstChild())
    {
    removeProperty(firstChild());
    }
  }
