#include "GCButtonManipulator.h"
#include "spropertyinformationhelpers.h"
#include "3D/GCCamera.h"
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
      const GCVisualManipulator *toRender,
      const QPoint &,
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

  virtual void render(const GCVisualManipulator *,
      const GCCamera *,
      XRenderer *)
    {
    /*XTransform t = camera->getPixelScaleFacingTransform(toRender->worldCentre().translation());

    r->pushTransform(t);
    r->setShader(&_shader);
    r->drawGeometry(_geo);
    r->setShader(0);
    r->popTransform();*/
    }

private:
  XGeometry _geo;
  XShader _shader;
  };

S_IMPLEMENT_PROPERTY(GCButtonManipulator)

void GCButtonManipulator::createTypeInformation(SPropertyInformation *info, const SPropertyInformationCreateData &data)
  {
  if(data.registerAttributes)
    {
    info->add(&GCButtonManipulator::checked, "checked");
    info->add(&GCButtonManipulator::checkable, "checkable");
    }
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
  qDebug() << "Click";
  }
