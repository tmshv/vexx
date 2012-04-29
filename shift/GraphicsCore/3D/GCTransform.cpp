#include "3D/GCTransform.h"
#include "styperegistry.h"
#include "Manipulators/GCTranslateManipulator.h"
#include "sdatabase.h"
#include "XLine.h"
#include "XFrustum.h"

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

void GCTransform::intersect(const XLine& line, Selector *s) const
  {
  XLine lineCpy(line);
  lineCpy.transform(transform());

  GCRenderArray::intersect(lineCpy, s);
  }

void GCTransform::intersect(const XFrustum& frus, Selector *s) const
  {
  XFrustum frusCpy(frus);
  frusCpy.transform(transform());

  GCRenderArray::intersect(frusCpy, s);
  }

