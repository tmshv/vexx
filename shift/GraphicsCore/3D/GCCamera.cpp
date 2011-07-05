#include "GCCamera.h"
#include "styperegistry.h"
#include "XRenderer.h"
#include "XCamera.h"

S_IMPLEMENT_PROPERTY(GCCamera)

SPropertyInformation *GCCamera::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCCamera>("GCCamera");
  return info;
  }

GCCamera::GCCamera()
  {
  }

void GCCamera::begin(XRenderer *r) const
  {
  XPerspectiveCamera c(45.0f, XVector3D(10,5,10));

  r->setProjectionTransform(c.projectionTransform());

  r->pushTransform(transform());
  }


void GCCamera::end(XRenderer *r) const
  {
  r->popTransform();
  }
