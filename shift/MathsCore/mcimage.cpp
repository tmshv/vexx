#include "mcimage.h"
#include "QImage"

void MCImage::computeImageOutput(const SPropertyInstanceInformation* inst, MCImage *image)
  {
  QImage imInput(image->filename());

  MCMathsOperation::ComputeLock l(&image->output);
  if(imInput.isNull())
    {
    l.data()->load(XMathsOperation::None, 0, 0, 0, 0, XMatrix3x3::Identity());
    }

  XMatrix3x3 transform = XMatrix3x3::Identity();
  bool useShuffle = true;

  xsize channels = 3;
  if(imInput.hasAlphaChannel())
    {
    channels = 4;
    if(image->premultiplied())
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
    if(image->premultiplied())
      {
      imInput.convertToFormat(QImage::Format_ARGB32_Premultiplied);
      }
    else
      {
      imInput.convertToFormat(QImage::Format_RGB888);
      useShuffle = false;
      }
    }

  const uchar *bits = imInput.bits();
  xAssert(bits);

  if(!useShuffle)
    {
    l.data()->load(XMathsOperation::Byte, (void*)bits, imInput.width(), imInput.height(), channels, transform);
    }
  else
    {
    xuint32 shuffleMask = XMathsOperation::shuffleMask(1, 0, 2, 3);

    xAssert(channels == 4);
    image->_preOperation.load(XMathsOperation::Byte, (void*)bits, imInput.width(), imInput.height(), channels, transform);
    l.data()->shuffle(image->_preOperation, shuffleMask);
    }
  }

S_IMPLEMENT_PROPERTY(MCImage)

SPropertyInformation *MCImage::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<MCImage>("MCImage");

  MCMathsOperation::InstanceInformation *outputInst = info->child(&MCImage::output);
  outputInst->setCompute(computeImageOutput);

  BoolProperty::InstanceInformation *preMultInst = info->add(&MCImage::premultiplied, "premultiplied");
  preMultInst->setAffects(outputInst);

  StringProperty::InstanceInformation *filenameInst = info->add(&MCImage::filename, "filename");
  filenameInst->setAffects(outputInst);

  return info;
  }

MCImage::MCImage()
  {
  }
