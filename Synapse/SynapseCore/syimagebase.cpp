#include "syimagebase.h"
#include "QImage"

S_IMPLEMENT_PROPERTY(SyImageBase)

SPropertyInformation *SyImageBase::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<SyImageBase>("SyImageBase");

  info->add(&SyImageBase::time, "time");
  info->add(&SyImageBase::operation, "operation");

  return info;
  }

SyImageBase::SyImageBase()
  {
  }

void SyImageBase::loadQImage(const QImage &imInput, bool premult)
  {
  SProfileFunction

  MCMathsOperation::ComputeLock l(&operation);
  if(imInput.isNull())
    {
    l.data()->load(XMathsOperation::None, 0, 0, 0, 0, 0, XMatrix3x3::Identity());
    }

  XMatrix3x3 transform = XMatrix3x3::Identity();

  xsize channels = 3;
  if(imInput.hasAlphaChannel())
    {
    channels = 4;
    if(premult)
      {
      imInput.convertToFormat(QImage::Format_ARGB32_Premultiplied);
      }
    else
      {
      imInput.convertToFormat(QImage::Format_ARGB32);
      }
    }
  else
    {
    if(premult)
      {
      imInput.convertToFormat(QImage::Format_ARGB32_Premultiplied);
      }
    else
      {
      imInput.convertToFormat(QImage::Format_RGB32);
      }
    }

  const uchar *bits = imInput.bits();

  if(bits)
    {
    xsize stride = imInput.bytesPerLine()/imInput.width();

    _preOperation.load(XMathsOperation::Byte, (void*)bits, stride, imInput.width(), imInput.height(), channels, transform);
    }
  else
    {
    _preOperation.load(XMathsOperation::None, 0, 0, 0, 0, channels, transform);
    }

  xuint32 shuffleMask = XMathsOperation::shuffleMask(2, 1, 0, 3);
  l.data()->shuffle(_preOperation, shuffleMask);
  }

QImage SyImageBase::asQImage() const
  {
  /*preGet();
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
  return im;*/
  return QImage();
  }
