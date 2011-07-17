#include "GCCamera.h"
#include "styperegistry.h"
#include "XRenderer.h"
#include "XCamera.h"

S_IMPLEMENT_PROPERTY(GCViewableTransform)

SPropertyInformation *GCViewableTransform::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCViewableTransform>("GCViewableTransform");

  info->add(&GCCamera::distance, "distance");
  info->add(&GCCamera::projection, "projection");

  return info;
  }

GCViewableTransform::GCViewableTransform()
  {
  }

void GCViewableTransform::setPosition(const XVector3D &point)
  {
  XTransform t = transform();
  t.translation() = point;
  transform = t;
  }

void GCViewableTransform::setFocalPoint(const XVector3D &point)
  {
  }

void GCViewableTransform::zoom(float factor, float x, float y)
  {
  qDebug() << "zoom" << factor;
  }

void GCViewableTransform::track(float x, float y)
  {
  qDebug() << "track" << x << y;
  }

void GCViewableTransform::dolly(float x, float y)
  {
  rotateAboutPoint(XVector3D(0, 0, 0), x, y);
  }

void GCViewableTransform::pan(float x, float y)
  {
  rotateAboutPoint(transform().translation(), x, y);
  }

void GCViewableTransform::rotateAboutPoint(const XVector3D &point, float x, float y)
  {
  qDebug() << "rap" << x << y;
  }

S_IMPLEMENT_PROPERTY(GCCamera)

SPropertyInformation *GCCamera::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCCamera>("GCCamera");
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
