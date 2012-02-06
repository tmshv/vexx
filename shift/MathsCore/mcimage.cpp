#include "mcimage.h"
#include "QImage"

void MCImage::computeImageOutput(const SPropertyInstanceInformation* inst, MCImage *image)
  {
  QImage imInput(image->filename());

  MCMathsOperation::ComputeLock l(&image->output);
  if(imInput.isNull())
    {
    l.data()->load(XMathsOperation::None, 0, 0, 0, 0, 0, XMatrix3x3::Identity());
    }

  XMatrix3x3 transform = XMatrix3x3::Identity();
  bool useShuffle = true;

  xsize channels = 3;
  if(imInput.hasAlphaChannel())
    {
    channels = 4;
    if(image->premultiply())
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
    if(image->premultiply())
      {
      imInput.convertToFormat(QImage::Format_ARGB32_Premultiplied);
      }
    else
      {
      imInput.convertToFormat(QImage::Format_RGB32);
      useShuffle = false;
      }
    }

  const uchar *bits = imInput.bits();
  xAssert(bits);

  xsize stride = imInput.bytesPerLine()/imInput.width();
  xuint32 shuffleMask = XMathsOperation::shuffleMask(2, 1, 0, 3);

  image->_preOperation.load(XMathsOperation::Byte, (void*)bits, stride, imInput.width(), imInput.height(), channels, transform);
  l.data()->shuffle(image->_preOperation, shuffleMask);
  }

S_IMPLEMENT_PROPERTY(MCImage)

SPropertyInformation *MCImage::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<MCImage>("MCImage");

  MCMathsOperation::InstanceInformation *outputInst = info->child(&MCImage::output);
  outputInst->setCompute(computeImageOutput);

  BoolProperty::InstanceInformation *preMultInst = info->add(&MCImage::premultiply, "premultiply");
  preMultInst->setAffects(outputInst);

  StringProperty::InstanceInformation *filenameInst = info->add(&MCImage::filename, "filename");
  filenameInst->setAffects(outputInst);

  return info;
  }

MCImage::MCImage()
  {
  }
