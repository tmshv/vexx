#include "GCComposeTransform.h"

void computeTransform(const SPropertyInstanceInformation *, GCComposeTransform *vec)
  {
  XTransform tr = XTransform::Identity();

  tr.translation() = vec->translationIn();

  TransformProperty::ComputeLock l(&vec->transformOut);
  *l.data() = tr;
  }

S_IMPLEMENT_PROPERTY(GCComposeTransform)

void GCComposeTransform::createTypeInformation(SPropertyInformation *info, const SPropertyInformationCreateData &data)
  {
  if(data.registerAttributes)
    {
    TransformProperty::InstanceInformation *transformInst = info->add(&GCComposeTransform::transformOut, "transformOut");
    transformInst->setCompute(computeTransform);

    Vector3DProperty::InstanceInformation *trInst = info->add(&GCComposeTransform::translationIn, "translationIn");

    trInst->setAffects(transformInst);
    }
  }

GCComposeTransform::GCComposeTransform()
  {
  }
