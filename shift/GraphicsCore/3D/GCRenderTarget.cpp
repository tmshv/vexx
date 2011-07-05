#include "GCRenderTarget.h"
#include "styperegistry.h"

S_IMPLEMENT_PROPERTY(GCRenderTarget)

SPropertyInformation *GCRenderTarget::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCRenderTarget>("GCRenderTarget");

  info->add(&GCRenderTarget::source, "source");

  return info;
  }

GCRenderTarget::GCRenderTarget()
  {
  }
