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

  if(result.dataType() != XMathsOperation::Byte)
    {
    xAssertFail();
    return false;
    }

  uchar* data = (uchar*)result.data();
  xsize w = result.dataWidth();
  xsize h = result.dataHeight();
  xsize channels = result.dataChannels();

  QImage::Format fmt = QImage::Format_RGB888;
  if(channels == 4)
    {
    fmt = QImage::Format_ARGB32;
    }

  QImage im(w, h, fmt);
  uchar *bits = im.bits();
  for(xuint32 y = 0; y < h; ++y)
    {
    uchar *scanline = bits + im.bytesPerLine();
    data += w*channels;
    for(xuint32 x = 0; x < w; ++x)
      {
      if(channels == 4)
        {
        scanline[0] = data[3];
        }

      scanline[1] = data[0];
      scanline[2] = data[1];
      scanline[3] = data[2];
      scanline += channels;
      }
    }

  im.save(filename);
  }

