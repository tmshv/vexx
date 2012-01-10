#include "GCViewport.h"

S_IMPLEMENT_PROPERTY(GCViewport)

SPropertyInformation *GCViewport::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCViewport>("GCViewport");

  info->add(&GCViewport::x, "x");
  info->add(&GCViewport::y, "y");

  return info;
  }

GCViewport::GCViewport()
  {
  }
