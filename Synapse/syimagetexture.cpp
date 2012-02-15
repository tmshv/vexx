#include "syimagetexture.h"

S_IMPLEMENT_PROPERTY(SyImageTexture)

SPropertyInformation *SyImageTexture::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<SyImageTexture>("SyImageTexture");

  GCTexture::InstanceInformation *textureInst = info->add(&SyImageTexture::texture, "texture");
  textureInst->setCompute(computeTexture);

  TransformProperty::InstanceInformation *transformInst = info->add(&SyImageTexture::transform, "transform");
  transformInst->setCompute(computeTransform);

  UnsignedIntProperty::InstanceInformation *widthInst = info->add(&SyImageTexture::width, "width");
  widthInst->setCompute(computeTransform);

  UnsignedIntProperty::InstanceInformation *heightInst = info->add(&SyImageTexture::height, "height");
  heightInst->setCompute(computeTransform);

  UnsignedIntProperty::InstanceInformation *imWidthInst = info->add(&SyImageTexture::imageWidth, "imageWidth");
  imWidthInst->setCompute(computeTransform);
  imWidthInst->setAffects(textureInst);

  UnsignedIntProperty::InstanceInformation *imHeightInst = info->add(&SyImageTexture::imageHeight, "imageHeight");
  imHeightInst->setCompute(computeTransform);
  imHeightInst->setAffects(textureInst);

  Vector2DProperty::InstanceInformation *imOffsetInst = info->add(&SyImageTexture::imageOffset, "imageOffset");
  imOffsetInst->setCompute(computeTransform);
  imOffsetInst->setAffects(textureInst);

  SyImageInput::InstanceInformation *inputInst = info->add(&SyImageTexture::input, "input");
  inputInst->setAffects(textureInst);

  GCViewableTransformPointer::InstanceInformation *viewerInst = info->add(&SyImageTexture::viewer, "viewer");

  const SPropertyInstanceInformation *affects[] = {
    transformInst,
    widthInst,
    heightInst,
    imWidthInst,
    imHeightInst,
    imOffsetInst
  };
  viewerInst->setAffects(affects, X_ARRAY_COUNT(affects));

  return info;
  }

SyImageTexture::SyImageTexture()
  {
  }

void SyImageTexture::computeTransform(const SPropertyInstanceInformation *, SyImageTexture *cont)
  {
  XVector3D lookAxis(0.0f, 0.0f, 1.0f);
  XVector3D xAxis(1.0f, 0.0f, 0.0f);
  XVector3D yAxis(0.0f, 1.0f, 0.0f);

  SPointerComputeLock cl(&cont->viewer);
  const GCViewableTransform* trans = cont->viewer.pointed();

  xuint32 vpW = trans->viewportWidth();
  xuint32 vpH = trans->viewportHeight();
  if(vpW > 0 && vpH > 0)
    {
    float dist = trans->transform().translation().dot(lookAxis);

    float x = trans->viewportX();
    float y = trans->viewportY();
    float w = trans->viewportWidth();
    float h = trans->viewportHeight();

    XVector3D tl = trans->worldSpaceAtDepthFromScreenSpace(x, y, dist);
    XVector3D tr = trans->worldSpaceAtDepthFromScreenSpace(x+w, y, dist);
    XVector3D bl = trans->worldSpaceAtDepthFromScreenSpace(x, y+h, dist);
    XVector3D br = trans->worldSpaceAtDepthFromScreenSpace(x+w, y+h, dist);

    TransformProperty::PODType imageTrans = TransformProperty::PODType::Identity();

    imageTrans.translation() = (tl + tr + bl + br) / 4.0f;

    cont->transform = imageTrans;
    XVector3D subA = tr - tl;
    XVector3D subB = br - bl;
    XVector3D subC = tl - bl;
    XVector3D subD = tr - br;

    float imW = ceilf(xMax(subA.dot(xAxis), subB.dot(xAxis)));
    float imH = ceilf(xMax(subC.dot(yAxis), subD.dot(yAxis)));

    cont->width = imW;
    cont->height = imH;

    float scaleX, scaleY;
    trans->approximatePixelSizeAtDistance(dist, scaleX, scaleY);

    XVector2D centre = imageTrans.translation().block<2, 1>(0, 0);
    cont->imageOffset = centre + XVector2D((-imW/2.0f)/scaleX, (-imH/2.0f)/scaleX);

    cont->imageWidth = trans->viewportWidth();
    cont->imageHeight = trans->viewportHeight();
    }
  }

void SyImageTexture::computeTexture(const SPropertyInstanceInformation *, SyImageTexture *cont)
  {
  GCTexture::ComputeLock l(&cont->texture);
  QImage im = cont->input.asQImage(cont->imageOffset().cast<xint32>(), cont->imageWidth(), cont->imageHeight());

  l.data()->load(im, XTexture::None);
  }
