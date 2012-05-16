#include "GCComposeTransform.h"
#include "spropertyinformationhelpers.h"
#include "XMacroHelpers"

void computeTransform(const SPropertyInstanceInformation *, GCComposeTransform *vec)
  {
  XTransform tr = XTransform::Identity();
  tr *= Eigen::AngleAxisf(X_DEGTORAD(vec->rotationAngleIn()), vec->rotationAxisIn());

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

    Vector3DProperty::InstanceInformation *axInst = info->add(&GCComposeTransform::rotationAxisIn, "rotationAxisIn");
    FloatProperty::InstanceInformation *angInst = info->add(&GCComposeTransform::rotationAngleIn, "rotationAngleIn");
    Vector3DProperty::InstanceInformation *trInst = info->add(&GCComposeTransform::translationIn, "translationIn");

    axInst->setAffects(transformInst);
    angInst->setAffects(transformInst);
    trInst->setAffects(transformInst);
    }
  }

GCComposeTransform::GCComposeTransform()
  {
  }
