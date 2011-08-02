#include "GCGeometryTransform.h"

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

void GCGeometryTransform::render(XRenderer* r)
  {
  }
