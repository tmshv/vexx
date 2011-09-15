#include "GCDistanceManipulator.h"
#include "3D/GCCamera.h"
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
    XModeller m(&_geo, 64);

    m.drawCube(XVector3D(0.1f, 0.0f, 0.0f), XVector3D(0.0f, 0.1f, 0.0f), XVector3D(0.0f, 0.0f, 0.1f));
    }

  virtual bool hitTest(
      const GCVisualManipulator *manip,
      const QPoint &,
      const GCCamera *camera,
      const XVector3D &clickDirection, // in world space
      float *distance)
    {
    const GCDistanceManipulator *toRender = manip->uncheckedCastTo<GCDistanceManipulator>();
    const XVector3D &camTrans = camera->transform().translation();
    XLine l(camTrans, clickDirection, XLine::PointAndDirection);

    XTransform wC = toRender->worldCentre();
    wC.translate(toRender->absoluteDisplacement());

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
      const GCCamera *camera,
      XRenderer *r)
    {
    r->pushTransform(toRender->worldCentre());
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

  info->add(&GCDistanceManipulator::scaleFactor, "scaleFactor");

  return info;
  }

GCDistanceManipulator::GCDistanceManipulator()
  {
  setDelegate(new DistanceDelegate());
  }

void GCDistanceManipulator::onDrag(const MouseMoveEvent &e)
  {
  GCLinearDragManipulator::onDrag(e);

  float rel = relativeDistance() * scaleFactor();

  qDebug() << rel;

  Pointer *p = driven.firstChild<Pointer>();
  while(p)
    {
    SProperty *input = p->input();
    if(!input)
      {
      continue;
      }
    FloatProperty *f = input->castTo<FloatProperty>();
    if(f)
      {
      *f = f->value() + rel;
      }
    else
      {
      xAssertFail();
      }
    p = p->nextSibling<Pointer>();
    }
  }
