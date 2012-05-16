#include "GCComposeVector3.h"
#include "spropertyinformationhelpers.h"

void computeVector(const SPropertyInstanceInformation *, GCComposeVector3 *vec)
  {
  Vector3DProperty::ComputeLock l(&vec->vectorOut);
  *l.data() = XVector3D(vec->xIn(), vec->yIn(), vec->zIn());
  }

S_IMPLEMENT_PROPERTY(GCComposeVector3)

void GCComposeVector3::createTypeInformation(SPropertyInformation *info, const SPropertyInformationCreateData &data)
  {
  if(data.registerAttributes)
    {
    Vector3DProperty::InstanceInformation *vectorInst = info->add(&GCComposeVector3::vectorOut, "vectorOut");
    vectorInst->setCompute(computeVector);

    FloatProperty::InstanceInformation *xInst = info->add(&GCComposeVector3::xIn, "xIn");
    FloatProperty::InstanceInformation *yInst = info->add(&GCComposeVector3::yIn, "yIn");
    FloatProperty::InstanceInformation *zInst = info->add(&GCComposeVector3::zIn, "zIn");

    xInst->setAffects(vectorInst);
    yInst->setAffects(vectorInst);
    zInst->setAffects(vectorInst);
    }
  }

GCComposeVector3::GCComposeVector3()
  {
  }
