#include "GCCamera.h"
#include "styperegistry.h"
#include "XRenderer.h"
#include "XCamera.h"
#include "Eigen/Geometry"

S_IMPLEMENT_ABSTRACT_PROPERTY(GCViewableTransform)

void computeView(const SPropertyInstanceInformation *, GCViewableTransform *tr)
  {
  XTransform inv;

  bool invertible = false;
  tr->transform().matrix().computeInverseWithCheck(inv.matrix(),invertible);
  xAssert(invertible);

  tr->viewTransform = inv;
  }

void computeInverseProjection(const SPropertyInstanceInformation *, GCViewableTransform *tr)
  {
  XTransform inv;

  bool invertible = false;
  tr->projection().matrix().computeInverseWithCheck(inv.matrix(),invertible);
  xAssert(invertible);

  tr->inverseProjection = inv;
  }

void GCViewableTransform::createTypeInformation(SPropertyInformation *info, const SPropertyInformationCreateData &data)
  {
  if(data.registerAttributes)
    {
    Vector3DProperty::InstanceInformation* upInfo = info->add(&GCViewableTransform::upVector, "upVector");
    upInfo->setDefault(XVector3D(0.0f, 1.0f, 0.0f));

    FloatProperty::InstanceInformation* focalInfo = info->add(&GCViewableTransform::focalDistance, "focalDistance");
    focalInfo->setDefault(1.0f);

    ComplexTransformProperty::InstanceInformation *invProjInfo = info->add(&GCViewableTransform::inverseProjection, "inverseProjection");
    invProjInfo->setCompute(computeInverseProjection);
    ComplexTransformProperty::InstanceInformation *projInfo = info->add(&GCViewableTransform::projection, "projection");
    projInfo->setAffects(invProjInfo);

    info->add(&GCViewableTransform::viewportX, "viewportX");
    info->add(&GCViewableTransform::viewportY, "viewportY");
    info->add(&GCViewableTransform::viewportWidth, "viewportWidth");
    info->add(&GCViewableTransform::viewportHeight, "viewportHeight");

    TransformProperty::InstanceInformation* viewInfo = info->add(&GCViewableTransform::viewTransform, "viewTransform");
    viewInfo->setCompute(computeView);

    TransformProperty::InstanceInformation* transformInfo = info->child(&GCViewableTransform::transform);
    transformInfo->setAffects(viewInfo);
    }

  if(data.registerInterfaces)
    {
    XInterface<GCViewableTransform> *ifc = info->apiInterface<GCViewableTransform>();

    XInterfaceBase::Function fn;
    fn = XScript::MethodToInCa<GCViewableTransform, void(float, float, float), &GCViewableTransform::setFocalPoint>::Call;
    ifc->addFunction("setFocalPoint", fn);

    fn = XScript::MethodToInCa<GCViewableTransform, void(float, float, float), &GCViewableTransform::setPosition>::Call;
    ifc->addFunction("setPosition", fn);
    }
  }

GCViewableTransform::GCViewableTransform() : _rotateEnabled(true)
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
  return transform().translation() - (transform().matrix().col(2).head<3>() * focalDistance());
  }

void GCViewableTransform::approximatePixelSizeAtDistance(float distanceFromCamera, float &xScale, float &yScale) const
  {
  xuint32 x = viewportX() + ((float)viewportWidth()/2.0f);
  xuint32 y = viewportY() + ((float)viewportHeight()/2.0f);

  XVector3D a = worldSpaceAtDepthFromScreenSpace(x, y, distanceFromCamera);
  XVector3D b = worldSpaceAtDepthFromScreenSpace(x+1, y, distanceFromCamera);
  XVector3D c = worldSpaceAtDepthFromScreenSpace(x, y+1, distanceFromCamera);

  xScale = (a - b).norm();
  yScale = (a - c).norm();
  }


XTransform GCViewableTransform::getPixelScaleFacingTransform(const XVector3D &worldPosition) const
  {
  XTransform t;

  t = transform();

  float pixelSizeX;
  float pixelSizeY;
  approximatePixelSizeAtDistance((worldPosition - t.translation()).norm(), pixelSizeX, pixelSizeY);

  t.scale(pixelSizeX);

  t.translation() = worldPosition;

  return t;
  }

bool GCViewableTransform::unitViewportCoordinates(xuint32 x, xuint32 y, float &xUnit, float &yUnit) const
  {
  x -= viewportX();
  y -= viewportY();
  if(x < 0 || y < 0 || x > viewportWidth() || y > viewportHeight())
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

  // viewport units are in positive down, flip to posisitve up
  yUnit *= -1.0f;

  return true;
  }

XVector3D GCViewableTransform::worldSpaceFromScreenSpace(xuint32 x, xuint32 y) const
  {
  XVector4D vpSpace(0.0f, 0.0f, 1.0f, 1.0f);
  unitViewportCoordinates(x, y, vpSpace(0), vpSpace(1));

  XVector4D downZAxisWorld = inverseProjection() * vpSpace;
  XVector3D world = transform() * downZAxisWorld.head<3>();
  return world;
  }

void GCViewableTransform::zoom(float factor, float, float)
  {
  XTransform t = transform();

  float moveDist = focalDistance() * -0.5f * (factor - 1.0f);
  focalDistance = focalDistance() - moveDist;

  // flip axes because input x and y are in a top left coordinate system
  XVector3D look = upVector().cross(XVector3D(1.0f, 0.0f, 0.0f)) * moveDist;

  t.translate(look);

  transform = t;
  }

void GCViewableTransform::track(float x, float y)
  {
  XTransform t = transform();

  float xScale;
  float yScale;
  approximatePixelSizeAtDistance(focalDistance(), xScale, yScale);

  // flip axes because input x and y are in a top left coordinate system
  XVector3D across = XVector3D(1.0f, 0.0f, 0.0f) * xScale;
  XVector3D up = upVector() * yScale;

  x *= -1.0f;

  t.translate(x * across + y * up);

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
  if(_rotateEnabled)
    {
    XTransform t = transform();

    // old translation vector
    float length = (t.translation() - point).norm();

    Eigen::AngleAxisf xRot(x * -0.005f, upVector());
    t.prerotate(xRot);

    Eigen::AngleAxisf yRot(y * -0.005f, XVector3D(1.0f, 0.0f, 0.0f));
    t.rotate(yRot);


    XVector3D newLook = t.matrix().col(2).head<3>();
    t.translation() = point + (newLook * length);

    transform = t;
    }
  }

S_IMPLEMENT_ABSTRACT_PROPERTY(GCCamera)

void GCCamera::createTypeInformation(SPropertyInformation *, const SPropertyInformationCreateData &)
  {
  }

GCCamera::GCCamera()
  {
  }

S_IMPLEMENT_PROPERTY(GCPerspectiveCamera)

void computePerspective(const SPropertyInstanceInformation *, GCPerspectiveCamera *c)
  {
  c->projection = XTransformUtilities::perspective(c->fieldOfView(), (float)c->viewportWidth() / (float)c->viewportHeight(), c->nearClip(), c->farClip());
  }

void GCPerspectiveCamera::createTypeInformation(SPropertyInformation *info, const SPropertyInformationCreateData &data)
  {
  if(data.registerAttributes)
    {
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
    }
  }

GCPerspectiveCamera::GCPerspectiveCamera()
  {
  }

XVector3D GCPerspectiveCamera::worldSpaceAtDepthFromScreenSpace(xuint32 x, xuint32 y, float depth) const
  {
  XVector3D wsFSS(worldSpaceFromScreenSpace(x,y));
  const XVector3D &camPos = transform().translation();

  XVector3D camToPoint = wsFSS - camPos;

  return camPos + (camToPoint.normalized() * depth);
  }
