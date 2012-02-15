#ifndef SYIMAGETEXTURE_H
#define SYIMAGETEXTURE_H

#include "sentity.h"
#include "syimageinput.h"
#include "GCBaseProperties.h"
#include "3D/GCCamera.h"
#include "3D/GCTexture.h"

class SyImageTexture : public SEntity
  {
  S_ENTITY(SyImageTexture, SEntity, 0);

public:
  SyImageTexture();

  SyImageInput input;
  GCViewableTransformPointer viewer;

  GCTexture texture;
  TransformProperty transform;
  UnsignedIntProperty width;
  UnsignedIntProperty height;

  Vector2DProperty imageOffset;
  UnsignedIntProperty imageWidth;
  UnsignedIntProperty imageHeight;

private:
  static void computeTransform(const SPropertyInstanceInformation *, SyImageTexture *cont);
  static void computeTexture(const SPropertyInstanceInformation *info, SyImageTexture *cont);
  };

#endif // SYIMAGETEXTURE_H
