#include "mcimage.h"
#include "spropertyinformationhelpers.h"
#include "QImage"

void MCImage::computeImageOutput(const SPropertyInstanceInformation* , MCImage *image)
  {
  QImage imInput(image->filename());

  MCMathsOperation::ComputeLock l(&image->output);
  if(imInput.isNull())
    {
    l.data()->load(XMathsOperation::None, 0, 0, 0, 0, 0, XVectorI2D::Zero());
    }

  XVectorI2D transform = XVectorI2D::Zero();
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

  if(bits)
    {
    xsize stride = imInput.bytesPerLine()/imInput.width();

    image->_preOperation.load(XMathsOperation::Byte, (void*)bits, stride, imInput.width(), imInput.height(), channels, transform);
    }
  else
    {
    image->_preOperation.load(XMathsOperation::None, 0, 0, 0, 0, channels, transform);
    }

  xuint32 shuffleMask = XMathsOperation::shuffleMask(2, 1, 0, 3);
  l.data()->shuffle(image->_preOperation, shuffleMask);
  }

S_IMPLEMENT_PROPERTY(MCImage, MathsCore)

void MCImage::createTypeInformation(SPropertyInformation *info, const SPropertyInformationCreateData &data)
  {
  if(data.registerAttributes)
    {
    MCMathsOperation::InstanceInformation *outputInst = info->child(&MCImage::output);
    outputInst->setCompute(computeImageOutput);

    BoolProperty::InstanceInformation *preMultInst = info->add(&MCImage::premultiply, "premultiply");
    preMultInst->setAffects(outputInst);

    StringProperty::InstanceInformation *filenameInst = info->add(&MCImage::filename, "filename");
    filenameInst->setAffects(outputInst);
    }
  }

MCImage::MCImage()
  {
  }
