#include "GCTranslateManipulator.h"
#include "3D/GCCamera.h"
#include "XModeller.h"
#include "XRenderer.h"
#include "XTransform.h"
#include "XShader.h"
#include "XLine.h"
#include "XCuboid.h"

class LinearTranslateDelegate : public GCVisualManipulator::Delegate
  {
public:
  LinearTranslateDelegate()
    {
    XModeller m(&_geo, 64);

    m.drawCube(XVector3D(0.1f, 0.0f, 0.0f), XVector3D(0.0f, 0.1f, 0.0f), XVector3D(0.0f, 0.0f, 0.1f));
    }

  virtual bool hitTest(
      const GCVisualManipulator *toRender,
      const QPoint &,
      const GCCamera *camera,
      const XVector3D &clickDirection, // in world space
      float *distance)
    {
    const XVector3D &camTrans = camera->transform().translation();
    XLine l(camTrans, clickDirection, XLine::PointAndDirection);

    const XTransform &wC = toRender->worldCentre();

    XMatrix4x4 t = wC.matrix();
    XMatrix4x4 tInv = t.inverse();
    XTransform lineTransform(tInv);
    l.transform(lineTransform);

    if(XMeshUtilities::intersect("vertex", l, _geo))
      {
      const XVector3D &wcTrans = wC.translation();
      *distance = (camTrans - wcTrans).norm() - 0.05f;
      return true;
      }

    return false;
    }

  virtual void render(const GCVisualManipulator *toRender,
      const GCCamera *,
      XRenderer *r)
    {
    const XTransform &wC = toRender->worldCentre();

    r->pushTransform(wC);
    r->setShader(&_shader);
    r->drawGeometry(_geo);
    r->setShader(0);
    r->popTransform();
    }

private:
  XGeometry _geo;
  XShader _shader;
  };

S_IMPLEMENT_PROPERTY(GCSingularTranslateManipulator)

SPropertyInformation *GCSingularTranslateManipulator::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCSingularTranslateManipulator>("GCSingularTranslateManipulator");

  return info;
  }

GCSingularTranslateManipulator::GCSingularTranslateManipulator()
  {
  setDelegate(new LinearTranslateDelegate());
  }

void GCSingularTranslateManipulator::addDriven(TransformProperty *in)
  {
  _driven << in;
  }

void GCSingularTranslateManipulator::onDrag(const MouseMoveEvent &e)
  {
  XVector3D relativeDisp;
  GCLinearDragManipulator::onDrag(e, relativeDisp);

  foreach(TransformProperty *t, _driven)
    {
    XTransform trans = t->value();
    trans.translation() += relativeDisp;
    *t = trans;
    }
  }


S_IMPLEMENT_PROPERTY(GCTranslateManipulator)

SPropertyInformation *GCTranslateManipulator::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCTranslateManipulator>("GCTranslateManipulator");

  info->add(&GCTranslateManipulator::central, "central");

  return info;
  }

GCTranslateManipulator::GCTranslateManipulator()
  {
  }

void GCTranslateManipulator::addDriven(TransformProperty *in)
  {
  central.addDriven(in);
  }
