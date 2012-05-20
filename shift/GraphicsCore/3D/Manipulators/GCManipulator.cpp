#include "GCManipulator.h"
#include "spropertyinformationhelpers.h"
#include "3D/GCCamera.h"
#include "XPlane.h"
#include "XLine.h"

S_IMPLEMENT_ABSTRACT_PROPERTY(GCVisualManipulator, GraphicsCore)

void GCVisualManipulator::createTypeInformation(SPropertyInformationTyped<GCVisualManipulator> *info,
                                                const SPropertyInformationCreateData &data)
  {
  if(data.registerAttributes)
    {
    info->add(&GCVisualManipulator::show, "show");
    info->add(&GCVisualManipulator::worldCentre, "worldCentre");
    info->add(&GCVisualManipulator::manipulatorsDisplayScale, "manipulatorsDisplayScale");
    }
  }

GCVisualManipulator::GCVisualManipulator() : _delegate(0)
  {
  }

void GCVisualManipulator::render(const GCCamera *camera, XRenderer *r) const
  {
  if(delegate())
    {
    delegate()->render(this, camera, r);
    }
  }

XVector3D GCVisualManipulator::focalPoint() const
  {
  if(delegate())
    {
    return delegate()->focalPoint(this);
    }
  return XVector3D::Zero();
  }

bool GCVisualManipulator::hitTest(
    const QPoint &widgetSpacePoint,
    const GCCamera *camera,
    const XVector3D &clickDirection, // in world space
    float *distance,
    GCVisualManipulator **clicked)
  {
  if(delegate())
    {
    if(delegate()->hitTest(this, widgetSpacePoint, camera, clickDirection, distance))
      {
      *clicked = this;
      return true;
      }
    }
  return false;
  }

S_IMPLEMENT_PROPERTY(GCVisualCompoundManipulator, GraphicsCore)

void GCVisualCompoundManipulator::createTypeInformation(SPropertyInformationTyped<GCVisualCompoundManipulator> *,
                                                        const SPropertyInformationCreateData &)
  {
  }

GCVisualCompoundManipulator::GCVisualCompoundManipulator()
  {
  }

bool GCVisualCompoundManipulator::hitTest(
    const QPoint &widgetSpacePoint,
    const GCCamera *camera,
    const XVector3D &clickDirection, // in world space
    float *distance,
    GCVisualManipulator **clicked)
  {
  xAssert(distance);
  xAssert(clicked);

  *clicked = 0;
  *distance = HUGE_VAL;

  float tempDistance = HUGE_VAL;
  for(GCVisualManipulator *m = firstChild<GCVisualManipulator>(); m; m = m->nextSibling<GCVisualManipulator>())
    {
    if(m->hitTest(widgetSpacePoint, camera, clickDirection, &tempDistance, clicked) &&
       tempDistance < *distance)
      {
      *distance = tempDistance;
      *clicked = m;
      }
    }
  return *clicked != 0;
  }

void GCVisualCompoundManipulator::render(const GCCamera *camera, XRenderer *r) const
  {
  for(GCVisualManipulator *m = firstChild<GCVisualManipulator>(); m; m = m->nextSibling<GCVisualManipulator>())
    {
    m->render(camera, r);
    }
  }

void GCVisualCompoundManipulator::onMouseClick(const MouseEvent &)
  {
  xAssertFail();
  }

void GCVisualCompoundManipulator::onMouseDoubleClick(const MouseEvent &)
  {
  xAssertFail();
  }

void GCVisualCompoundManipulator::onMouseDrag(const MouseMoveEvent &)
  {
  xAssertFail();
  }

void GCVisualCompoundManipulator::onMouseRelease(const MouseEvent &)
  {
  xAssertFail();
  }


S_IMPLEMENT_ABSTRACT_PROPERTY(GCVisualDragManipulator, GraphicsCore)

void GCVisualDragManipulator::createTypeInformation(SPropertyInformationTyped<GCVisualDragManipulator> *,
                                                    const SPropertyInformationCreateData &)
  {
  }

GCVisualDragManipulator::GCVisualDragManipulator()
  {
  }

void GCVisualDragManipulator::onMouseClick(const MouseEvent &)
  {
  }

void GCVisualDragManipulator::onMouseDoubleClick(const MouseEvent &)
  {
  }

void GCVisualDragManipulator::onMouseDrag(const MouseMoveEvent &e)
  {
  onDrag(e);
  }

void GCVisualDragManipulator::onMouseRelease(const MouseEvent &)
  {
  }


S_IMPLEMENT_ABSTRACT_PROPERTY(GCVisualClickManipulator, GraphicsCore)

void GCVisualClickManipulator::createTypeInformation(SPropertyInformationTyped<GCVisualClickManipulator> *,
                                                     const SPropertyInformationCreateData &)
  {
  }

GCVisualClickManipulator::GCVisualClickManipulator()
  {
  }

void GCVisualClickManipulator::onMouseClick(const MouseEvent &)
  {
  onClick();
  }

void GCVisualClickManipulator::onMouseDoubleClick(const MouseEvent &)
  {
  }

void GCVisualClickManipulator::onMouseDrag(const MouseMoveEvent &)
  {
  }

void GCVisualClickManipulator::onMouseRelease(const MouseEvent &)
  {
  }


S_IMPLEMENT_ABSTRACT_PROPERTY(GCLinearDragManipulator, GraphicsCore)

void GCLinearDragManipulator::createTypeInformation(SPropertyInformationTyped<GCLinearDragManipulator> *info,
                                                    const SPropertyInformationCreateData &data)
  {
  if(data.registerAttributes)
    {
    info->add(&GCLinearDragManipulator::lockMode, "lockMode");
    info->add(&GCLinearDragManipulator::lockDirection, "lockDirection");
    }
  }

GCLinearDragManipulator::GCLinearDragManipulator()
  {
  }

void GCLinearDragManipulator::onDrag(const MouseMoveEvent &e, XVector3D &rel)
  {
  rel = XVector3D::Zero();

  XVector3D focus = focalPoint();
  const XVector3D &camPosition = e.cam->transform().translation();
  float focalDistanceFromCamera = (camPosition - focus).norm();

  xuint32 lock = lockMode();
  if(lock == Linear)
    {
    XLine p(focus, lockDirection(), XLine::PointAndDirection);
    XLine a(camPosition, e.lastDirection, XLine::PointAndDirection);
    XLine b(camPosition, e.direction, XLine::PointAndDirection);

    float lastHitT = a.closestPointOn(p);
    float hitT = b.closestPointOn(p);

    xAssert(fabs(lastHitT) < HUGE_VAL);
    xAssert(fabs(hitT) < HUGE_VAL);

    if(lastHitT > 0.0f && lastHitT < HUGE_VAL &&
       hitT > 0.0f && hitT < HUGE_VAL)
      {
      XVector3D lastHit = a.sample(lastHitT);
      XVector3D hit = b.sample(hitT);

      float lastPT = p.closestPointTo(lastHit);
      float pT = p.closestPointTo(hit);

      rel = p.sample(pT) - p.sample(lastPT);
      xAssert(fabs(rel.normalized().dot(p.direction())) > 0.97f);

      if(rel.norm() > HUGE_VAL)
        {
        xAssertFail();
        }
      }
    }
  else if(lock == Planar)
    {
    XPlane p(focus, lockDirection());
    XLine a(camPosition, e.lastDirection, XLine::PointAndDirection);
    XLine b(camPosition, e.direction, XLine::PointAndDirection);

    XVector3D lastHit = a.sample(p.intersection(a));
    XVector3D hit = b.sample(p.intersection(b));

    rel = hit - lastHit;
    }
  else // Free.
    {
    XVector3D a = camPosition + e.lastDirection*focalDistanceFromCamera;
    XVector3D b = camPosition + e.direction*focalDistanceFromCamera;

    rel = b - a;
    }
  }
