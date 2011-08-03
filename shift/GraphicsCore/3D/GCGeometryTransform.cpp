#include "GCGeometryTransform.h"
#include "XRenderer.h"

S_IMPLEMENT_PROPERTY(GCGeometryTransform)

SPropertyInformation *GCGeometryTransform::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCGeometryTransform>("GCGeometryTransform");

  info->add(&GCGeometryTransform::geometry, "geometry");

  return info;
  }

GCGeometryTransform::GCGeometryTransform()
  {
  }

void GCGeometryTransform::render(XRenderer* r) const
  {
  const GCGeometry *geo = geometry();

  if(geo)
    {
    r->pushTransform(transform());
    r->drawGeometry(geo->runtimeGeometry());
    r->popTransform();
    }
  }
