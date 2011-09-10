#include "GCTextureRenderTarget.h"
#include "styperegistry.h"

S_IMPLEMENT_PROPERTY(GCTextureRenderTarget)

SPropertyInformation *GCTextureRenderTarget::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCTextureRenderTarget>("GCTextureRenderTarget");
  return info;
  }

GCTextureRenderTarget::GCTextureRenderTarget()
  {
  }
