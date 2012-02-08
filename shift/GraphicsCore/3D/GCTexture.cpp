#include "GCTexture.h"

S_IMPLEMENT_PROPERTY(GCTexture)


SPropertyInformation *GCTexture::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCTexture>("GCTexture");

  return info;
  }

GCTexture::GCTexture()
  {
  QImage im(5, 5, QImage::Format_ARGB32_Premultiplied);

  im.fill(Qt::red);

  _texture.load(im, XTexture::None);
  }
