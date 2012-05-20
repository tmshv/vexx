#include "GCTexture.h"
#include "spropertyinformationhelpers.h"

S_IMPLEMENT_TYPED_POINTER_TYPE(GCTexturePointer, GraphicsCore)

S_IMPLEMENT_PROPERTY(GCTexture, GraphicsCore)

void GCTexture::createTypeInformation(SPropertyInformationTyped<GCTexture> *,
                                      const SPropertyInformationCreateData &)
  {
  }

GCTexture::GCTexture()
  {
  }
