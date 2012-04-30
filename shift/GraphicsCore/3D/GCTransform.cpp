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

class InternalSelector : public GCRenderable::Selector
  {
public:
  InternalSelector(const XTransform &tr, Selector *parent)
      : _parent(parent), _transform(tr)
    {
    }

  void onHit(const XVector3D &point, const XVector3D &normal, GCRenderable *renderable)
    {
    _parent->onHit(_transform * point, _transform.linear() * normal, renderable);
    }

private:
  GCRenderable::Selector *_parent;
  XTransform _transform;
  };

void GCTransform::intersect(const XLine& line, Selector *s)
  {
  const XTransform &tr = transform();

  XLine lineCpy(line);
  lineCpy.transform(tr.inverse());

  InternalSelector sel(tr, s);
  GCRenderArray::intersect(lineCpy, &sel);
  }

void GCTransform::intersect(const XFrustum& frus, Selector *s)
  {
  const XTransform &tr = transform();

  XFrustum frusCpy(frus);
  frusCpy.transform(tr.inverse());

  InternalSelector sel(tr, s);
  GCRenderArray::intersect(frusCpy, &sel);
  }

