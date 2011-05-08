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

  xsize bytesPerPixel = imageIn.depth()/8;
  const quint8 *pixel = imageIn.bits();
  for(int i = 0; i < height; ++i )
    {
    xsize rowPos = i * width;
    for(int j = 0; j < width; ++j)
      {
      data[rowPos+j] = (float)*pixel/255.0f;
      pixel += bytesPerPixel;
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

  xsize bytesPerPixel = im.depth()/8;
  quint8 *pixel = im.bits();
  const float *constData = image.get();
  for(int i = 0; i < height; ++i )
    {
    int rowPos = i*width;
    for(int j = 0; j < width; ++j)
      {
      pixel[0] = constData[rowPos+j] * 255.0f;
      pixel += bytesPerPixel;
      }
    }
  return im;
  }
