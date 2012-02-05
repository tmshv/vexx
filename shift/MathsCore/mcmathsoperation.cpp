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
  for(xuint32 y = 0; y < h; ++y)
    {
    uchar *scanline = bits + y * im.bytesPerLine();
    xAssert(im.bytesPerLine()/w == channels);
    for(xuint32 x = 0; x < w; ++x)
      {
      xuint8 id = 0;
      if(channels == 4)
        {
        scanline[id++] = data[3];
        }

      scanline[id++] = data[0];
      scanline[id++] = data[1];
      scanline[id++] = data[2];
      scanline += channels;
      data = (xuint32*)((xuint8*)data + stride);
      }
    }

  im.save(filename);
  }

