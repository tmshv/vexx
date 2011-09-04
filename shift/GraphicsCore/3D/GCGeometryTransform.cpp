#include "GCGeometryTransform.h"
#include "XRenderer.h"

S_IMPLEMENT_PROPERTY(GCGeometryTransform)

SPropertyInformation *GCGeometryTransform::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCGeometryTransform>("GCGeometryTransform");

  info->add(&GCGeometryTransform::geometry, "geometry");

  info->addInheritedInterface<GCGeometryTransform, GCManipulatable>();

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

void GCGeometryTransform::addManipulators(SPropertyArray *a, const GCTransform *tr)
  {
  xAssert(tr == 0);
  GCTransform::addManipulators(a, tr);

  const GCGeometry *geo = geometry();
  if(geo)
    {
    SProperty *geoOwner = geo->parent();
    GCManipulatable *manipulatorInterface = geoOwner->interface<GCManipulatable>();
    if(manipulatorInterface)
      {
      manipulatorInterface->addManipulators(a, this);
      }
    }
  }
