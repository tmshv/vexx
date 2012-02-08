#include "GCTexture.h"

S_IMPLEMENT_PROPERTY(GCTexture)


SPropertyInformation *GCTexture::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCTexture>("GCTexture");

  return info;
  }

GCTexture::GCTexture()
  {
  QImage im("C:\\Users\\Public\\Pictures\\Sample Pictures\\Koala.jpg");

  _texture.load(im, XTexture::None);
  }
