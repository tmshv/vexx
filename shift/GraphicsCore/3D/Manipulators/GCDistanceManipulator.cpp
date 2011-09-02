#include "GCDistanceManipulator.h"
#include "XModeller.h"
#include "XRenderer.h"
#include "XTransform.h"
#include "XShader.h"
#include "XLine.h"
#include "XCuboid.h"

class DistanceDelegate : public GCVisualManipulator::Delegate
  {
public:
  DistanceDelegate()
    {
    XTransform t;
    t = Eigen::Translation3f(10.0f, 10.0f, 0.0f);

    XModeller m(&_geo, 64);
    m.setTransform(t);

    m.drawQuad(XVector3D(20.0f, 0.0f, 0.0f), XVector3D(0.0f, 20.0f, 0.0f));
    }

  virtual bool hitTest(
      GCVisualManipulator *toRender,
      const QPoint &widgetSpacePoint,
      const GCCamera *camera,
      const XVector3D &clickDirection, // in world space
      float *distance)
    {
    XLine l(camera->transform().translation(), clickDirection, XLine::PointAndDirection);

    XMatrix4x4 t = camera->getPixelScaleFacingTransform(toRender->worldCentre().translation()).matrix();
    XMatrix4x4 tInv = t.inverse();

    XTransform lineTransform(tInv);

    l.transform(lineTransform);

    XCuboid c(XVector3D(0.0f, 0.0f, 0.0f), XVector3D(20.0f, 20.0f, 0.1f));

    if(c.intersects(l, *distance))
      {
      *distance *= t.row(0).norm();
      return true;
      }

    return false;
    }

  virtual void render(GCVisualManipulator *toRender,
      const GCCamera *camera,
      XRenderer *r)
    {
    XTransform t = camera->getPixelScaleFacingTransform(toRender->worldCentre().translation());

    r->pushTransform(t);
    r->setShader(&_shader);
    r->drawGeometry(_geo);
    r->setShader(0);
    r->popTransform();
    }

private:
  XGeometry _geo;
  XShader _shader;
  };

S_IMPLEMENT_PROPERTY(GCDistanceManipulator)

SPropertyInformation *GCDistanceManipulator::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCDistanceManipulator>("GCDistanceManipulator");

  return info;
  }

GCDistanceManipulator::GCDistanceManipulator()
  {
  setDelegate(new DistanceDelegate());
  }

void GCDistanceManipulator::onDrag(
    const QPoint &oldWidgetSpacePoint,
    const QPoint &widgetSpacePoint,
    const XVector3D &cameraPosition,
    const XVector3D &oldClickDirection,
    const XVector3D &clickDirection)
  {
  }
