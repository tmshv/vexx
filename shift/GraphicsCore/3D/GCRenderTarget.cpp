#include "GCRenderTarget.h"
#include "styperegistry.h"

S_IMPLEMENT_PROPERTY(GCRenderTarget)

void computeAspectRatio(const SPropertyInstanceInformation *, GCRenderTarget *vp)
  {
  vp->aspectRatio = (float)vp->width() / (float)vp->height();
  }


SPropertyInformation *GCRenderTarget::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCRenderTarget>("GCRenderTarget");

  GCRenderablePointer::InstanceInformation *sourceInst = info->add(&GCRenderTarget::source, "source");
  sourceInst->setMode(SPropertyInstanceInformation::Input);

  FloatProperty::InstanceInformation* aR = info->add(&GCRenderTarget::aspectRatio, "aspectRatio");
  aR->setCompute(computeAspectRatio);
  aR->setDefault(1.0f);

  UnsignedIntProperty::InstanceInformation *width = info->add(&GCRenderTarget::width, "width");
  width->setAffects(aR);
  width->setMode(SPropertyInstanceInformation::Output);
  UnsignedIntProperty::InstanceInformation *height = info->add(&GCRenderTarget::height, "height");
  height->setAffects(aR);
  height->setMode(SPropertyInstanceInformation::Output);

  return info;
  }

GCRenderTarget::GCRenderTarget()
  {
  }
