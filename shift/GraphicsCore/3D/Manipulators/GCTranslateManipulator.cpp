#include "GCTranslateManipulator.h"
#include "3D/GCCamera.h"
#include "XModeller.h"
#include "XRenderer.h"
#include "XTransform.h"
#include "XShader.h"
#include "XLine.h"
#include "XCuboid.h"

class CentralTranslateDelegate : public GCVisualManipulator::Delegate
  {
public:
  CentralTranslateDelegate()
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


class LinearTranslateDelegate : public GCVisualManipulator::Delegate
  {
public:
  LinearTranslateDelegate()
    {
    XModeller m(&_geo, 64);

    m.begin(XModeller::Lines);
      m.vertex(0.0f, 0.0f, 0.0f);
      m.vertex(1.0f, 0.0f, 0.0f);
    m.end();

    m.drawCone(XVector3D(0.8f, 0.0f, 0.0f), XVector3D(1.0f, 0.0f, 0.0f), 0.2f, 0.06f);

    _shader.setType(XShader::ColourShader);
    }

  virtual bool hitTest(
      const GCVisualManipulator *manip,
      const QPoint &,
      const GCCamera *camera,
      const XVector3D &clickDirection, // in world space
      float *distance)
    {
    const GCSingularTranslateManipulator *toRender = manip->uncheckedCastTo<GCSingularTranslateManipulator>();

    const XVector3D &camTrans = camera->transform().translation();
    XLine clickLine(camTrans, clickDirection, XLine::PointAndDirection);

    XLine manipLine(XVector3D(0.0f, 0.0f, 0.0f), toRender->lockDirection().normalized());

    const XTransform &wC = toRender->worldCentre();
    manipLine.transform(wC);

    float clickT = clickLine.closestPointOn(manipLine);
    XVector3D clickPt = clickLine.sample(clickT);

    float manipT = manipLine.closestPointTo(clickPt);
    if(manipT >= 0.0f && manipT <= 1.0f)
      {
      const float maxPixels = 5.0f;

      float worldDist = (manipLine.sample(manipT) - clickPt).norm();

      *distance = (clickPt - camTrans).norm();

      float pixelSizeX, pixelSizeY;
      camera->approximatePixelSizeAtDistance(*distance, pixelSizeX, pixelSizeY);

      float maxWorld = maxPixels * pixelSizeX;

      if(worldDist >= 0.0f && worldDist < maxWorld)
        {
        return true;
        }
      }

    return false;
    }

  virtual void render(const GCVisualManipulator *manip,
      const GCCamera *,
      XRenderer *r)
    {
    const GCSingularTranslateManipulator *toRender = manip->uncheckedCastTo<GCSingularTranslateManipulator>();

    XTransform wC = toRender->worldCentre();

    XVector3D x = XVector3D(1.0f, 0.0f, 0.0f);
    XVector3D lockDir = toRender->lockDirection().normalized();
    if(lockDir.dot(x) < 0.99f)
      {
      XVector3D dir = x.cross(lockDir);
      Eigen::AngleAxisf a((float)M_PI/2.0f, dir);

      wC.rotate(a);
      }

    XVector4D col;
    col(3) = 1.0f;
    col.head<3>() = toRender->lockDirection();

    _shader.getVariable("colour")->setValue(col);

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

void postCreateTranslateManip(GCTranslateManipulator *m)
  {
  m->worldCentre.connect(&m->x.worldCentre);
  m->x.setDelegate(new LinearTranslateDelegate());

  m->x.lockMode = GCSingularTranslateManipulator::Linear;
  m->x.lockDirection = XVector3D(1.0f, 0.0f, 0.0f);


  m->worldCentre.connect(&m->y.worldCentre);
  m->y.setDelegate(new LinearTranslateDelegate());

  m->y.lockMode = GCSingularTranslateManipulator::Linear;
  m->y.lockDirection = XVector3D(0.0f, 1.0f, 0.0f);


  m->worldCentre.connect(&m->z.worldCentre);
  m->z.setDelegate(new LinearTranslateDelegate());

  m->z.lockMode = GCSingularTranslateManipulator::Linear;
  m->z.lockDirection = XVector3D(0.0f, 0.0f, 1.0f);


  m->worldCentre.connect(&m->central.worldCentre);
  m->central.setDelegate(new CentralTranslateDelegate());
  }

SPropertyInformation *GCTranslateManipulator::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCTranslateManipulator>("GCTranslateManipulator", postCreateTranslateManip);

  info->add(&GCTranslateManipulator::x, "x");
  info->add(&GCTranslateManipulator::y, "y");
  info->add(&GCTranslateManipulator::z, "z");
  info->add(&GCTranslateManipulator::central, "central");

  return info;
  }

GCTranslateManipulator::GCTranslateManipulator()
  {
  }

void GCTranslateManipulator::addDriven(TransformProperty *in)
  {
  central.addDriven(in);
  x.addDriven(in);
  y.addDriven(in);
  z.addDriven(in);
  }
