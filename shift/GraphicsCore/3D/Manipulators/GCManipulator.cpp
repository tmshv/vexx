#include "GCManipulator.h"
#include "3D/GCCamera.h"
#include "XPlane.h"
#include "XLine.h"

S_IMPLEMENT_ABSTRACT_PROPERTY(GCVisualManipulator)

SPropertyInformation *GCVisualManipulator::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCVisualManipulator>("GCVisualManipulator");

  info->add(&GCVisualManipulator::show, "show");
  info->add(&GCVisualManipulator::worldCentre, "worldCentre");
  info->add(&GCVisualManipulator::manipulatorsDisplayScale, "manipulatorsDisplayScale");

  return info;
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

S_IMPLEMENT_PROPERTY(GCVisualCompoundManipulator)

SPropertyInformation *GCVisualCompoundManipulator::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCVisualCompoundManipulator>("GCVisualCompoundManipulator");

  info->add(&GCVisualCompoundManipulator::childManipulators, "childManipulators");

  return info;
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
  for(GCVisualManipulator *m = childManipulators.firstChild<GCVisualManipulator>(); m; m = m->nextSibling<GCVisualManipulator>())
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

void GCVisualCompoundManipulator::render(const GCCamera *camera, XRenderer *r)
  {
  for(GCVisualManipulator *m = childManipulators.firstChild<GCVisualManipulator>(); m; m = m->nextSibling<GCVisualManipulator>())
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


S_IMPLEMENT_ABSTRACT_PROPERTY(GCVisualDragManipulator)

SPropertyInformation *GCVisualDragManipulator::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCVisualDragManipulator>("GCVisualDragManipulator");

  return info;
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


S_IMPLEMENT_ABSTRACT_PROPERTY(GCVisualClickManipulator)

SPropertyInformation *GCVisualClickManipulator::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCVisualClickManipulator>("GCVisualClickManipulator");

  return info;
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


S_IMPLEMENT_ABSTRACT_PROPERTY(GCLinearDragManipulator)

SPropertyInformation *GCLinearDragManipulator::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCLinearDragManipulator>("GCLinearDragManipulator");

  info->add(&GCLinearDragManipulator::lockMode, "lockMode");
  info->add(&GCLinearDragManipulator::lockDirection, "lockDirection");

  info->add(&GCLinearDragManipulator::absoluteDisplacement, "absoluteDisplacement");

  return info;
  }

GCLinearDragManipulator::GCLinearDragManipulator()
  {
  }

void GCLinearDragManipulator::onDrag(const MouseMoveEvent &e, XVector3D &rel)
  {
  XVector3D focus = focalPoint();
  const XVector3D &camPosition = e.cam->transform().translation();
  float focalDistanceFromCamera = (camPosition - focus).norm();

  xuint32 lock = lockMode();
  if(lock == Linear)
    {
    XLine p(focus, lockDirection(), XLine::PointAndDirection);
    XLine a(camPosition, e.lastDirection, XLine::PointAndDirection);
    XLine b(camPosition, e.direction, XLine::PointAndDirection);

    XVector3D lastHit = p.sample(p.closestPointOn(a));
    XVector3D hit = p.sample(p.closestPointOn(b));

    rel = hit - lastHit;
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
