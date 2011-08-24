#ifndef GCMANIPULATOR_H
#define GCMANIPULATOR_H

#include "GCGlobal.h"
#include "sproperty.h"

class XRenderer;

class GRAPHICSCORE_EXPORT GCManipulator : public SProperty
  {
  S_ABSTRACT_ENTITY(GCViewableTransform, SProperty, 0)

public:
  GCManipulator();
  };

class GRAPHICSCORE_EXPORT GCVisualManipulator : public GCManipulator
  {
  S_ABSTRACT_ENTITY(GCVisualManipulator, GCManipulator, 0)

public:
  GCVisualManipulator();
  
  BoolProperty show;
  Vector3Property worldPointOfInterest;
  FloatProperty manipulatorsDisplayScale;
  
  FloatProperty screenSpaceScale;
  
  virtual bool hitTest(const QPoint &widgetSpacePoint, const XVector3D &cameraPosition, const XVector3D &clickDirection, SProperty **clicked) = 0;
  virtual void render3D(XRenderer *) { }
  virtual void render2D(QPainter *) { }
  };

class GRAPHICSCORE_EXPORT GCVisualDragManipulator : public GCManipulator
  {
  S_ABSTRACT_ENTITY(GCVisualDragManipulator, GCVisualManipulator, 0)

public:
  GCVisualDragManipulator();
  
  virtual void onDrag(
    const QPoint &oldWidgetSpacePoint,
    const QPoint &widgetSpacePoint,
    const XVector3D &cameraPosition,
    const XVector3D &oldClickDirection,
    const XVector3D &clickDirection);
  };

class GRAPHICSCORE_EXPORT GCVisualClickManipulator : public GCManipulator
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

class GRAPHICSCORE_EXPORT GCButtonManipulator : public GCVisualClickManipulator
  {
  S_ABSTRACT_ENTITY(GCButtonManipulator, GCVisualClickManipulator, 0)

public:
  GCButtonManipulator();

  BoolProperty checked;
  BoolProperty checkable;
  
  virtual void onClick();
  };

#endif // GCMANIPULATOR_H
