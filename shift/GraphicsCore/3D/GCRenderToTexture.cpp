#include "GCRenderToTexture.h"
#include "styperegistry.h"

S_IMPLEMENT_PROPERTY(GCRenderToTexture)

const SPropertyInformation *GCRenderToTexture::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCRenderToTexture>("GCRenderToTexture");
  return info;
  }

GCRenderToTexture::GCRenderToTexture()
{
}
