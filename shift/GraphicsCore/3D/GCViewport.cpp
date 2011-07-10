#include "GCViewport.h"

S_IMPLEMENT_PROPERTY(GCViewport)

void computeAspectRatio(const SPropertyInstanceInformation *child, SPropertyContainer *prop)
  {
  GCViewport *vp = prop->uncheckedCastTo<GCViewport>();
  vp->aspectRatio = (float)vp->width() / (float)vp->height();
  }

SPropertyInformation *GCViewport::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCViewport>("GCViewport");

  FloatProperty::InstanceInformation* aR = info->add(&GCViewport::aspectRatio, "aspectRatio");
  aR->setCompute(computeAspectRatio);
  aR->setDefault(1.0f);

  UnsignedIntProperty::InstanceInformation *width = info->add(&GCViewport::width, "width");
  width->setAffects(aR);
  UnsignedIntProperty::InstanceInformation *height = info->add(&GCViewport::height, "height");
  height->setAffects(aR);

  return info;
  }

GCViewport::GCViewport()
  {
  }
