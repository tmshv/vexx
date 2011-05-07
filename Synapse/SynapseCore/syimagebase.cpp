#include "syimagebase.h"
#include "QImage"

S_PROPERTY_CONTAINER_DEFINITION(SyImageBase, SPropertyContainer)
    S_PROPERTY_DEFINITION(SFloatArrayProperty, image)
S_PROPERTY_CONTAINER_END_DEFINITION(SyImageBase, SPropertyContainer, saveContainer, loadContainer)

SyImageBase::SyImageBase()
  {
  }

void SyImageBase::loadQImage(const QImage &imageIn)
  {
  int width = imageIn.width();
  int height = imageIn.height();

  image.resize(width, height);

  for(int i = 0; i < height; ++i )
    {
    for(int j = 0; j < width; ++j)
      {
      image.setIndex(j, i, (float)qRed(imageIn.pixel(j, i))/255.0f);
      }
    }
  postSet();
  }

QImage SyImageBase::asQImage() const
  {
  preGet();
  int width = image.width();
  int height = image.height();

  QImage im(width, height, QImage::Format_ARGB32);
  im.fill(qRgba(0,0,0,255));

  quint8 *data = im.bits();
  for(int i = 0; i < height; ++i )
    {
    for(int j = 0; j < width; ++j)
      {
      quint8 *pixel(data + (i*width) + j);
      pixel[0] = image.atIndex(j, i) * 255.0f;
      }
    }
  return im;
  }
