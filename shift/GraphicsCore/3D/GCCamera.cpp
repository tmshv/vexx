#include "GCCamera.h"
#include "styperegistry.h"
#include "XRenderer.h"
#include "XCamera.h"

S_IMPLEMENT_PROPERTY(GCViewableTransform)

void computeView(const SPropertyInstanceInformation *info, SPropertyContainer *ent)
  {
  GCViewableTransform* tr = ent->uncheckedCastTo<GCViewableTransform>();

  XTransform inv;
  bool invertible = false;
  tr->transform().matrix().computeInverseWithCheck(inv.matrix(),invertible);
  xAssert(invertible);
  tr->viewTransform = inv;
  }

SPropertyInformation *GCViewableTransform::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCViewableTransform>("GCViewableTransform");

  Vector3DProperty::InstanceInformation* upInfo = info->add(&GCViewableTransform::upVector, "upVector");
  upInfo->setDefault(XVector3D(0.0f, 1.0f, 0.0f));

  FloatProperty::InstanceInformation* focalInfo = info->add(&GCViewableTransform::focalDistance, "focalDistance");
  focalInfo->setDefault(1.0f);

  info->add(&GCViewableTransform::projection, "projection");

  TransformProperty::InstanceInformation* viewInfo = info->add(&GCViewableTransform::viewTransform, "viewTransform");
  viewInfo->setCompute(computeView);

  TransformProperty::InstanceInformation* transformInfo = info->child(&GCViewableTransform::transform);
  transformInfo->setAffects(viewInfo);

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

void GCViewableTransform::setFocalPoint(const XVector3D &aim)
  {
  XTransform t = transform();

  XVector3D forward = (aim - t.translation()).normalized();
  XVector3D side = forward.cross(upVector()).normalized();
  XVector3D up = side.cross(forward);

  t.matrix().col(0).head<3>() = side;
  t.matrix().col(1).head<3>() = up;
  t.matrix().col(2).head<3>() = -forward;

  focalDistance = (aim - t.translation()).norm();
  transform = t;
  }

XVector3D GCViewableTransform::focalPoint() const
  {
  return transform().translation() + (transform().matrix().row(2).head<3>().transpose() * focalDistance());
  }

void GCViewableTransform::zoom(float factor, float x, float y)
  {
  XTransform t = transform();
  XTransform v = viewTransform();

  float moveDist = focalDistance() * 0.1f * (factor - 1.0f);
  focalDistance = focalDistance() - moveDist;

  // flip axes because input x and y are in a top left coordinate system
  XVector3D look = v.matrix().col(2).head<3>() * moveDist;;

  t.translate(look);


  transform = t;
  }

void GCViewableTransform::track(float x, float y)
  {
  XTransform t = transform();
  XTransform v = viewTransform();

  float fDScale = focalDistance() * 0.005f;

  // flip axes because input x and y are in a top left coordinate system
  XVector3D across = v.matrix().col(0).head<3>() * -fDScale * x;
  XVector3D up = v.matrix().col(1).head<3>() * fDScale * y;

  t.translate(across + up);

  transform = t;
  }

void GCViewableTransform::dolly(float x, float y)
  {
  rotateAboutPoint(focalPoint(), x, y);
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
