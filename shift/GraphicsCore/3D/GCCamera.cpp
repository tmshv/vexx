#include "GCCamera.h"
#include "styperegistry.h"
#include "XRenderer.h"
#include "XCamera.h"
#include "Eigen/Geometry"

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

  info->add(&GCViewableTransform::viewportX, "viewportX");
  info->add(&GCViewableTransform::viewportY, "viewportY");
  info->add(&GCViewableTransform::viewportWidth, "viewportWidth");
  info->add(&GCViewableTransform::viewportHeight, "viewportHeight");

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

bool GCViewableTransform::unitViewportCoordinates(xuint32 x, xuint32 y, float &xUnit, float &yUnit)
  {
  x -= viewportX();
  y -= viewportY();
  if(x < 0 || y < 0 || x >= viewportWidth() || y >= viewportHeight())
    {
    xUnit = 0.0f;
    yUnit = 0.0f;
    return false;
    }

  xUnit = (float)x/viewportWidth();
  yUnit = (float)y/viewportHeight();

  xUnit *= 2.0f;
  yUnit *= 2.0f;

  xUnit -= 1.0f;
  yUnit -= 1.0f;

  return true;
  }

XVector3D GCViewableTransform::worldSpaceFromScreenSpace(xuint32 x, xuint32 y)
  {
  XVector3D vpSpace(0.0f, 0.0f, 1.0f);
  unitViewportCoordinates(x, y, vpSpace(0), vpSpace(1));

  XVector3D world = transform() * vpSpace;
  return world;
  }

void GCViewableTransform::zoom(float factor, float x, float y)
  {
  XTransform t = transform();
  XTransform v = viewTransform();

  float moveDist = focalDistance() * -0.5f * (factor - 1.0f);
  focalDistance = focalDistance() - moveDist;

  // flip axes because input x and y are in a top left coordinate system
  XVector3D look = v.matrix().col(2).head<3>() * moveDist;;

  t.translate(look);


  transform = t;
  }

void GCViewableTransform::track(float x, float y)
  {
  XTransform t = transform();

  xuint32 midWidth = viewportWidth()/2;
  xuint32 midHeight = viewportWidth()/2;

  XVector3D posA = worldSpaceAtDepthFromScreenSpace(midWidth, midHeight, focalDistance());
  XVector3D posB = worldSpaceAtDepthFromScreenSpace(midWidth+x, midHeight, focalDistance());
  XVector3D posC = worldSpaceAtDepthFromScreenSpace(midWidth, midHeight-y, focalDistance());

  float xScale = (posA - posB).norm();
  float yScale = (posA - posC).norm();

  // flip axes because input x and y are in a top left coordinate system
  XVector3D across = t.matrix().col(0).head<3>() * xScale;
  XVector3D up = t.matrix().col(1).head<3>() * yScale;

  if(x > 0)
    {
    across *= -1.0f;
    }

  if(y < 0)
    {
    up *= -1.0f;
    }

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
  XTransform t = transform();

  // old translation vector
  XVector3D vec = t.translation() - point;
  float length = vec.norm();
  vec = vec.normalized();

  Eigen::AngleAxisf xRot(x * 0.001f, upVector());
  t.rotate(xRot);

  Eigen::AngleAxisf yRot(y * 0.001f, t.matrix().col(0).head<3>());
  t.rotate(yRot);

  t.translation() = point + (vec * length);

  transform = t;
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
  c->projection = XTransformUtilities::perspective(c->fieldOfView(), (float)c->viewportWidth() / (float)c->viewportHeight(), c->nearClip(), c->farClip());
  }

SPropertyInformation *GCPerspectiveCamera::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCPerspectiveCamera>("GCPerspectiveCamera");

  ComplexTransformProperty::InstanceInformation *proj = info->child(&GCCamera::projection);
  proj->setCompute(computePerspective);

  UnsignedIntProperty::InstanceInformation *width = info->child(&GCCamera::viewportWidth);
  width->setAffects(proj);
  UnsignedIntProperty::InstanceInformation *height = info->child(&GCCamera::viewportHeight);
  height->setAffects(proj);

  FloatProperty::InstanceInformation *fov = info->add(&GCPerspectiveCamera::fieldOfView, "fieldOfView");
  fov->setDefault(45.0f);
  fov->setAffects(proj);

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

XVector3D GCPerspectiveCamera::worldSpaceAtDepthFromScreenSpace(xuint32 x, xuint32 y, float depth)
  {
  XVector3D wsFSS(worldSpaceFromScreenSpace(x,y));
  const XVector3D &camPos = transform().translation();

  XVector3D camToPoint = wsFSS - camPos;

  return camPos + (camToPoint.normalized() * depth);
  }
