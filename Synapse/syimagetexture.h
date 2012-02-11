#ifndef SYIMAGETEXTURE_H
#define SYIMAGETEXTURE_H

#include "sentity.h"
#include "syimageinput.h"
#include "3D/GCTexture.h"

class SyImageTexture : public SEntity
  {
  S_ENTITY(SyImageTexture, SEntity, 0);

public:
  SyImageTexture();

  SyImageInput input;
  GCTexture texture;
  UnsignedIntProperty width;
  UnsignedIntProperty height;

private:
  static void computeTexture(const SPropertyInstanceInformation *info, SyImageTexture *cont);
  };

#endif // SYIMAGETEXTURE_H
