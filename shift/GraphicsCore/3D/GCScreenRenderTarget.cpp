#include "GCScreenRenderTarget.h"
#include "styperegistry.h"

S_IMPLEMENT_PROPERTY(GCScreenRenderTarget)

SPropertyInformation *GCScreenRenderTarget::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCScreenRenderTarget>("GCScreenRenderTarget");
  return info;
  }

GCScreenRenderTarget::GCScreenRenderTarget()
  {
  }
