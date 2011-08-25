#ifndef GCMANIPULATOR_H
#define GCMANIPULATOR_H

#include "GCGlobal.h"
#include "sproperty.h"

class XRenderer;

class GRAPHICSCORE_EXPORT GCVisualManipulator : public SPropertyContainer
  {
  S_ABSTRACT_ENTITY(GCVisualManipulator, GCManipulator, 0)

public:
  GCVisualManipulator();
  
  BoolProperty show;
  TransformProperty worldCentre;
  FloatProperty manipulatorsDisplayScale;
  
  FloatProperty screenSpaceScale;
  
  virtual bool hitTest(
    const QPoint &widgetSpacePoint,
    const XVector3D &cameraPosition, // in world space
    const XVector3D &clickDirection, // in world space
    GCVisualManipulator **clicked) = 0;
    
  virtual void render3D(XRenderer *) { }
  virtual void render2D(QPainter *) { }
  
  virtual void onMouseClick(QPoint widgetPoint) = 0;
  virtual void onMouseDoubleClick(QPoint widgetPoint) = 0;
  virtual void onMouseDrag(QPoint lastWidgetPoint, QPoint widgetPoint) = 0;
  virtual void onMouseRelease(QPoint widgetPoint) = 0;
  };
  
class GRAPHICSCORE_EXPORT GCVisualCompoundManipulator : public GCVisualManipulator
  {
  S_ABSTRACT_ENTITY(GCVisualCompoundManipulator, GCVisualManipulator, 0)

public:
  GCVisualCompoundManipulator();
  
  SPropertyArray childManipulators;
  
  virtual bool hitTest(
    const QPoint &widgetSpacePoint,
    const XVector3D &cameraPosition, // in world space
    const XVector3D &clickDirection, // in world space
    GCVisualManipulator **clicked);
    
  virtual void render3D(XRenderer *);
  virtual void render2D(QPainter *);
  };


class GRAPHICSCORE_EXPORT GCVisualDragManipulator : public GCVisualManipulator
  {
  S_ABSTRACT_ENTITY(GCVisualDragManipulator, GCVisualManipulator, 0)

public:
  GCVisualDragManipulator();
  
  virtual void onDrag(
    const QPoint &oldWidgetSpacePoint,
    const QPoint &widgetSpacePoint,
    const XVector3D &cameraPosition, // in world space
    const XVector3D &oldClickDirection, // in world space
    const XVector3D &clickDirection); // in world space
  };

class GRAPHICSCORE_EXPORT GCVisualClickManipulator : public GCVisualManipulator
  {
  S_ABSTRACT_ENTITY(GCVisualClickManipulator, GCVisualManipulator, 0)

public:
  GCVisualClickManipulator();
  
  virtual void onClick() = 0;
  };

class GRAPHICSCORE_EXPORT GCButtonManipulator : public GCVisualClickManipulator
  {
  S_ABSTRACT_ENTITY(GCButtonManipulator, GCVisualClickManipulator, 0)

public:
  GCButtonManipulator();

  BoolProperty checked;
  BoolProperty checkable;
  
  virtual void onClick();
  };

class GRAPHICSCORE_EXPORT GCLinearDragManipulator : public GCVisualDragManipulator
  {
  S_ABSTRACT_ENTITY(GCLinearDragManipulator, GCVisualDragManipulator, 0)

public:
  GCLinearDragManipulator();

  // distance moved from click point in world space
  FloatProperty absoluteDistance;
  // distance moved last drag in world space
  FloatProperty relativeDistance;
  
  virtual void onDrag(
    const QPoint &oldWidgetSpacePoint,
    const QPoint &widgetSpacePoint,
    const XVector3D &cameraPosition,
    const XVector3D &oldClickDirection,
    const XVector3D &clickDirection);
  };

#endif // GCMANIPULATOR_H
