#include "mcmathsoperation.h"
#include "QImage"

S_IMPLEMENT_PROPERTY(MCMathsOperation)

SPropertyInformation *MCMathsOperation::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<MCMathsOperation>("MCMathsOperation");

  info->addAPIMemberFunction<bool, MCMathsOperation, QString, &MCMathsOperation::saveResultToFile>("saveResultToFile");

  return info;
  }

bool MCMathsOperation::saveResultToFile(QString filename)
  {
  XMathsResult result(value());

  if(result.dataType() == XMathsOperation::None)
    {
    qWarning() << "Saving invalid operation";
    return false;
    }

  if(result.dataType() != XMathsOperation::UnsignedInt)
    {
    xAssertFail();
    return false;
    }

  xuint32* data = (xuint32*)result.data();
  xsize stride = result.dataStride();
  xsize w = result.dataWidth();
  xsize h = result.dataHeight();
  xsize channels = result.dataChannels();

  QImage::Format fmt = QImage::Format_RGB888;
  if(channels == 4)
    {
    fmt = QImage::Format_ARGB32;
    }

  if(!data)
    {
    xAssertFail();
    return false;
    }

  QImage im(w, h, fmt);
  uchar *bits = im.bits();
  xAssert(im.bytesPerLine()/w == 4);
  for(xuint32 y = 0; y < h; ++y)
    {
    QRgb *scanline = (QRgb*)(bits + y * im.bytesPerLine());
    for(xuint32 x = 0; x < w; ++x)
      {
      xuint32 alpha = 255;
      if(channels == 4)
        {
        alpha = data[3];
        }

      *scanline = qRgba(data[0], data[1], data[2], alpha);
      scanline++;
      data = (xuint32*)((xuint8*)data + stride);
      }
    }

  im.save(filename);
  }

