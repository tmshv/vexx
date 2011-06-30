#include "GCRenderTarget.h"
#include "styperegistry.h"

S_IMPLEMENT_PROPERTY(GCRenderTarget)

const SPropertyInformation *GCRenderTarget::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCRenderTarget>("GCRenderTarget");
  return info;
  }

GCRenderTarget::GCRenderTarget()
{
}
