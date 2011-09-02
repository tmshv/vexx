#include "GCManipulator.h"

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

void GCVisualManipulator::render(const GCCamera *camera, XRenderer *r)
  {
  if(delegate())
    {
    delegate()->render(this, camera, r);
    }
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

void GCVisualCompoundManipulator::onMouseClick(QPoint, const GCCamera *, const XVector3D &)
  {
  xAssertFail();
  }

void GCVisualCompoundManipulator::onMouseDoubleClick(QPoint, const GCCamera *, const XVector3D &)
  {
  xAssertFail();
  }

void GCVisualCompoundManipulator::onMouseDrag(QPoint, QPoint, const GCCamera *, const XVector3D &, const XVector3D &)
  {
  xAssertFail();
  }

void GCVisualCompoundManipulator::onMouseRelease(QPoint, const GCCamera *, const XVector3D &)
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

void GCVisualDragManipulator::onMouseClick(QPoint widgetPoint, const GCCamera *, const XVector3D &)
  {
  }

void GCVisualDragManipulator::onMouseDoubleClick(QPoint widgetPoint, const GCCamera *, const XVector3D &)
  {
  }

void GCVisualDragManipulator::onMouseDrag(QPoint lastWidgetPoint, QPoint widgetPoint, const GCCamera *, const XVector3D &, const XVector3D &)
  {
  }

void GCVisualDragManipulator::onMouseRelease(QPoint widgetPoint, const GCCamera *, const XVector3D &)
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

void GCVisualClickManipulator::onMouseClick(QPoint, const GCCamera *, const XVector3D &)
  {
  onClick();
  }

void GCVisualClickManipulator::onMouseDoubleClick(QPoint, const GCCamera *, const XVector3D &)
  {
  }

void GCVisualClickManipulator::onMouseDrag(QPoint, QPoint, const GCCamera *, const XVector3D &, const XVector3D &)
  {
  }

void GCVisualClickManipulator::onMouseRelease(QPoint, const GCCamera *, const XVector3D &)
  {
  }


S_IMPLEMENT_ABSTRACT_PROPERTY(GCLinearDragManipulator)

SPropertyInformation *GCLinearDragManipulator::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCLinearDragManipulator>("GCLinearDragManipulator");

  info->add(&GCLinearDragManipulator::lockMode, "lockMode");
  info->add(&GCLinearDragManipulator::lockDirection, "lockDirection");

  info->add(&GCLinearDragManipulator::relativeDistance, "relativeDistance");
  info->add(&GCLinearDragManipulator::absoluteDistance, "absoluteDistance");

  return info;
  }

GCLinearDragManipulator::GCLinearDragManipulator()
  {
  }
