#include "3D/GCTransform.h"
#include "styperegistry.h"
#include "Manipulators/GCTranslateManipulator.h"
#include "sdatabase.h"

S_IMPLEMENT_PROPERTY(GCTransform)

void GCTransform::createTypeInformation(SPropertyInformation *info, const SPropertyInformationCreateData &data)
  {
  if(data.registerAttributes)
    {
    TransformProperty::InstanceInformation* trInfo = info->add(&GCTransform::transform, "transform");
    trInfo->setDefault(XTransform::Identity());
    }
  }

GCTransform::GCTransform()
  {
  }

void GCTransform::addManipulators(SPropertyArray *a, const GCTransform *tr)
  {
  xAssert(tr == 0);

  GCTranslateManipulator *manip = a->add<GCTranslateManipulator>();

  manip->addDriven(&transform);

  transform.connect(&manip->worldCentre);
  }
