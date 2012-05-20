#include "GCComposeVector3.h"
#include "spropertyinformationhelpers.h"

void computeVector(const SPropertyInstanceInformation *, GCComposeVector3 *vec)
  {
  Vector3DProperty::ComputeLock l(&vec->vectorOut);
  *l.data() = XVector3D(vec->xIn(), vec->yIn(), vec->zIn());
  }

S_IMPLEMENT_PROPERTY(GCComposeVector3, GraphicsCore)

void GCComposeVector3::createTypeInformation(SPropertyInformationTyped<GCComposeVector3> *info,
                                             const SPropertyInformationCreateData &data)
  {
  if(data.registerAttributes)
    {
    auto vectorInst = info->add(&GCComposeVector3::vectorOut, "vectorOut");
    vectorInst->setCompute<computeVector>();

    auto xInst = info->add(&GCComposeVector3::xIn, "xIn");
    auto yInst = info->add(&GCComposeVector3::yIn, "yIn");
    auto zInst = info->add(&GCComposeVector3::zIn, "zIn");

    xInst->setAffects(vectorInst);
    yInst->setAffects(vectorInst);
    zInst->setAffects(vectorInst);
    }
  }

GCComposeVector3::GCComposeVector3()
  {
  }
