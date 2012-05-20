#include "GCRenderTarget.h"
#include "styperegistry.h"
#include "spropertyinformationhelpers.h"

S_IMPLEMENT_PROPERTY(GCRenderTarget, GraphicsCore)

void computeAspectRatio(const SPropertyInstanceInformation *, GCRenderTarget *vp)
  {
  vp->aspectRatio = (float)vp->width() / (float)vp->height();
  }


void GCRenderTarget::createTypeInformation(SPropertyInformationTyped<GCRenderTarget> *info,
                                           const SPropertyInformationCreateData &data)
  {
  if(data.registerAttributes)
    {
    auto sourceInst = info->add(&GCRenderTarget::source, "source");
    sourceInst->setMode(SPropertyInstanceInformation::InternalInput);

    auto aR = info->add(&GCRenderTarget::aspectRatio, "aspectRatio");
    aR->setCompute<computeAspectRatio>();
    aR->setDefault(1.0f);

    auto width = info->add(&GCRenderTarget::width, "width");
    width->setAffects(aR);
    width->setMode(SPropertyInstanceInformation::Output);

    auto height = info->add(&GCRenderTarget::height, "height");
    height->setAffects(aR);
    height->setMode(SPropertyInstanceInformation::Output);
    }
  }

GCRenderTarget::GCRenderTarget()
  {
  }
