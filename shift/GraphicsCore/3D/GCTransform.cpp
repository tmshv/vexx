#include "3D/GCTransform.h"
#include "styperegistry.h"
#include "Manipulators/GCTranslateManipulator.h"
#include "sdatabase.h"

void TransformProperty::assignProperty(const SProperty *f, SProperty *t)
  {
  TransformProperty *to = t->uncheckedCastTo<TransformProperty>();

  const TransformProperty *sProp = f->castTo<TransformProperty>();
  if(sProp)
    {
    to->assign(sProp->value());
    return;
    }

  const ComplexTransformProperty *tProp = f->castTo<ComplexTransformProperty>();
  if(tProp)
    {
    to->assign(XTransform(tProp->value().matrix()));
    return;
    }
  }

void ComplexTransformProperty::assignProperty(const SProperty *f, SProperty *t)
  {
  ComplexTransformProperty *to = t->uncheckedCastTo<ComplexTransformProperty>();

  const ComplexTransformProperty *tProp = f->castTo<ComplexTransformProperty>();
  if(tProp)
    {
    to->assign(tProp->value());
    return;
    }

  const TransformProperty *sProp = f->castTo<TransformProperty>();
  if(sProp)
    {
    to->assign(XComplexTransform(sProp->value().matrix()));
    return;
    }

  }

S_IMPLEMENT_PROPERTY(GCTransform)

SPropertyInformation *GCTransform::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCTransform>("GCTransform");

  TransformProperty::InstanceInformation* trInfo = info->add(&GCTransform::transform, "transform");
  trInfo->setDefault(XTransform::Identity());

  return info;
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
