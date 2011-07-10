#include "GCCamera.h"
#include "styperegistry.h"
#include "XRenderer.h"
#include "XCamera.h"

S_IMPLEMENT_PROPERTY(GCCamera)

SPropertyInformation *GCCamera::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCCamera>("GCCamera");

  info->add(&GCCamera::projection, "projection");

  TransformProperty::InstanceInformation *trInfo = info->child(&GCCamera::transform);
  trInfo->setDefault(XTransformUtilities::lookAt(XVector3D(10,5,10), XVector3D(0,0,0), XVector3D(0,1,0)));

  return info;
  }

GCCamera::GCCamera()
  {
  }

S_IMPLEMENT_PROPERTY(GCPerspectiveCamera)

void computePerspective(const SPropertyInstanceInformation *child, SPropertyContainer *prop)
  {
  GCPerspectiveCamera *c = prop->uncheckedCastTo<GCPerspectiveCamera>();
  c->projection = XTransformUtilities::perspective(c->fieldOfView(), c->aspectRatio(), c->nearClip(), c->farClip());
  }

SPropertyInformation *GCPerspectiveCamera::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCPerspectiveCamera>("GCPerspectiveCamera");

  ComplexTransformProperty::InstanceInformation *proj = info->child(&GCCamera::projection);
  proj->setCompute(computePerspective);

  FloatProperty::InstanceInformation *fov = info->add(&GCPerspectiveCamera::fieldOfView, "fieldOfView");
  fov->setDefault(45.0f);
  fov->setAffects(proj);

  FloatProperty::InstanceInformation *aR = info->add(&GCPerspectiveCamera::aspectRatio, "aspectRatio");
  aR->setDefault(1.0f);
  aR->setAffects(proj);

  FloatProperty::InstanceInformation *nC = info->add(&GCPerspectiveCamera::nearClip, "nearClip");
  nC->setDefault(0.1f);
  nC->setAffects(proj);

  FloatProperty::InstanceInformation *fC = info->add(&GCPerspectiveCamera::farClip, "farClip");
  fC->setDefault(100.0f);
  fC->setAffects(proj);

  return info;
  }

GCPerspectiveCamera::GCPerspectiveCamera()
  {
  }
