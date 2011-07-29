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
  if(size() > 1)
    {
    removeProperty(firstChild());
    }

  addProperty(type);
  }

void GCGeometryAttribute::addPolygons(const xuint32 *sizes, xuint32 count)
  {
  xuint32 expandBy = 0;
  for(xuint32 i=0; i<count; ++i)
    {
    expandBy += 1 + sizes[i];
    }

  SUIntArrayProperty::EigenArray data = polygons.data();
  xuint32 offset = data.size();

  data.resize(offset + expandBy, 1);

  for(xuint32 i=0; i<count; ++i)
    {
    data(offset) = sizes[i];
    offset += 1;

    // initialise indices to 0.
    for(xuint32 j=0; j<sizes[i]; ++i)
      {
      data(offset + j) = 0;
      }

    offset += sizes[i];
    }

  polygons.setData(data);
  }

void GCGeometryAttribute::removePolygons(xuint32 index, xuint32 count)
  {
  SUIntArrayProperty::EigenArray data = polygons.data();

  xuint32 currentIndex = 0;

  xuint32 startOffset = X_UINT32_SENTINEL;

  xuint32 initialSize = data.size();
  xuint32 offset = 0;
  while(offset < initialSize)
    {
    if(index == currentIndex && startOffset == X_UINT32_SENTINEL)
      {
      startOffset = offset;
      }
    else if(index == currentIndex+count)
      {
      xAssert(startOffset != X_UINT32_SENTINEL);

      // offset of the last index we are removing
      xuint32 finalOffset = offset + data(offset);

      // assign the important data over the old data.
      data.block(startOffset, 0, initialSize - finalOffset, 1) = data.block(finalOffset, 0, initialSize - finalOffset, 1);

      // remove the end of the array we dont want.
      data.resize(initialSize - (finalOffset - startOffset), 1);

      polygons.setData(data);
      return;
      }

    // increment by the poly size and the indices.
    offset += 1 + data(offset);
    currentIndex += 1;
    }

  xAssertFail();
  }

void GCGeometryAttribute::setPolygon(xuint32 index, const xuint32 *indices)
  {
  SUIntArrayProperty::EigenArray data = polygons.data();

  xuint32 currentIndex = 0;

  xuint32 initialSize = data.size();
  xuint32 offset = 0;
  while(offset < initialSize)
    {
    if(index == currentIndex)
      {
      xuint32* rawData = data.data();
      data += offset + 1;

      memcpy(rawData, indices, sizeof(xuint32) * data(offset));

      polygons.setData(data);
      return;
      }

    // increment by the poly size and the indices.
    offset += 1 + data(offset);
    currentIndex += 1;
    }

  xAssertFail();
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
