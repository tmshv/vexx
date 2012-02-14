#include "syimagetexture.h"

S_IMPLEMENT_PROPERTY(SyImageTexture)

SPropertyInformation *SyImageTexture::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<SyImageTexture>("SyImageTexture");

  GCTexture::InstanceInformation *textureInst = info->add(&SyImageTexture::texture, "texture");
  textureInst->setCompute(computeTexture);

  UnsignedIntProperty::InstanceInformation *widthInst = info->add(&SyImageTexture::width, "width");
  widthInst->setCompute(computeTexture);

  UnsignedIntProperty::InstanceInformation *heightInst = info->add(&SyImageTexture::height, "height");
  heightInst->setCompute(computeTexture);

  SyImageInput::InstanceInformation *inputInst = info->add(&SyImageTexture::input, "input");

  const SPropertyInstanceInformation *affects[] = {
    textureInst,
    widthInst,
    heightInst
  };

  inputInst->setAffects(affects, X_ARRAY_COUNT(affects));

  return info;
  }

SyImageTexture::SyImageTexture()
  {
  }

void SyImageTexture::computeTexture(const SPropertyInstanceInformation *, SyImageTexture *cont)
  {
  GCTexture::ComputeLock l(&cont->texture);
  QImage im = cont->input.asQImage();

  l.data()->load(im, XTexture::None);
  cont->width = im.width();
  cont->height = im.height();
  }
