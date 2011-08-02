#include "GCRenderToScreen.h"
#include "styperegistry.h"

S_IMPLEMENT_PROPERTY(GCRenderToScreen)

SPropertyInformation *GCRenderToScreen::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCRenderToScreen>("GCRenderToScreen");
  return info;
  }

GCRenderToScreen::GCRenderToScreen()
  {
  }
