#include "GCGeometryTransform.h"
#include "XRenderer.h"

S_IMPLEMENT_PROPERTY(GCGeometryTransform)

void GCGeometryTransform::createTypeInformation(SPropertyInformation *info, const SPropertyInformationCreateData &data)
  {
  if(data.registerAttributes)
    {
    info->add(&GCGeometryTransform::geometry, "geometry");
    }

  if(data.registerInterfaces)
    {
    info->addInheritedInterface<GCGeometryTransform, GCManipulatable>();
    }
  }

GCGeometryTransform::GCGeometryTransform()
  {
  }

void GCGeometryTransform::render(XRenderer* r) const
  {
  GCProfileFunction
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
