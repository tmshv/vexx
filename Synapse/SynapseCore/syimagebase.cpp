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
  SProfileFunction
  int width = imageIn.width();
  int height = imageIn.height();

  XVector<float> data;
  data.resize(width * height);

  for(int i = 0; i < height; ++i )
    {
    int rowPos = i*width;
    for(int j = 0; j < width; ++j)
      {
      data[rowPos+j] = (float)qRed(imageIn.pixel(j, i))/255.0f;
      }
    }

  image.set(width, height, data);

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
