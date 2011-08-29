#include "GCButtonManipulator.h"
#include "XModeller.h"
#include "XRenderer.h"
#include "XTransform.h"
#include "XShader.h"
#include "XLine.h"
#include "XCuboid.h"

class ButtonDelegate : public GCVisualManipulator::Delegate
  {
public:
  ButtonDelegate()
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

    XTransform t = camera->getPixelScaleFacingTransform(toRender->worldCentre().translation());
    l.transform(t);

    XCuboid c(XVector3D(0.0f, 0.0f, 0.0f), XVector3D(20.0f, 20.0f, 0.1f));

    return c.intersects(l, *distance);
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

S_IMPLEMENT_PROPERTY(GCButtonManipulator)

SPropertyInformation *GCButtonManipulator::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCButtonManipulator>("GCButtonManipulator");

  info->add(&GCButtonManipulator::checked, "checked");
  info->add(&GCButtonManipulator::checkable, "checkable");

  return info;
  }

GCButtonManipulator::GCButtonManipulator()
  {
  setDelegate(new ButtonDelegate());
  }

GCButtonManipulator::~GCButtonManipulator()
  {
  delete delegate();
  setDelegate(0);
  }

void GCButtonManipulator::onClick()
  {
  }
