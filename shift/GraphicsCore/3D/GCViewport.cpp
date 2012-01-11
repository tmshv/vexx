#include "GCViewport.h"

S_IMPLEMENT_PROPERTY(GCViewport)

SPropertyInformation *GCViewport::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCViewport>("GCViewport");

  UnsignedIntProperty::InstanceInformation *xInst = info->add(&GCViewport::x, "x");
  xInst->setMode(SPropertyInstanceInformation::Output);
  UnsignedIntProperty::InstanceInformation *yInst = info->add(&GCViewport::y, "y");
  yInst->setMode(SPropertyInstanceInformation::Output);

  return info;
  }

GCViewport::GCViewport()
  {
  }
