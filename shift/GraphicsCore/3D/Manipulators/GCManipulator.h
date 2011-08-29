#ifndef GCMANIPULATOR_H
#define GCMANIPULATOR_H

#include "GCGlobal.h"
#include "spropertycontainer.h"
#include "sbaseproperties.h"
#include "3D/GCTransform.h"
#include "3D/GCCamera.h"

class XRenderer;
class QPainter;

class GRAPHICSCORE_EXPORT GCManipulatable : public SInterfaceBase
  {
  S_INTERFACE_TYPE(ManipulatableInterface)
public:
  virtual void addManipulators(SPropertyArray *) = 0;
  };

class GRAPHICSCORE_EXPORT GCVisualManipulator : public SPropertyContainer
  {
  S_ABSTRACT_PROPERTY_CONTAINER(GCVisualManipulator, SPropertyContainer, 0)

public:
  class Delegate
    {
  public:
    virtual ~Delegate() { }

    virtual bool hitTest(
      GCVisualManipulator *toRender,
      const QPoint &widgetSpacePoint,
      const GCCamera *camera,
      const XVector3D &clickDirection, // in world space
      float *distance) = 0;

    virtual void render(GCVisualManipulator *toRender,
                        const GCCamera *camera,
                        XRenderer *) = 0;
    };

XProperties:
  XProperty(Delegate *, delegate, setDelegate);

public:
  GCVisualManipulator();
  
  BoolProperty show;
  TransformProperty worldCentre;
  FloatProperty manipulatorsDisplayScale;
  
  virtual bool hitTest(
    const QPoint &widgetSpacePoint,
    const GCCamera *camera,
    const XVector3D &clickDirection, // in world space
    float *distance,
    GCVisualManipulator **clicked);
    
  virtual void render(const GCCamera *camera, XRenderer *);
  
  virtual void onMouseClick(QPoint widgetPoint, const GCCamera *cam, const XVector3D &direction) = 0;
  virtual void onMouseDoubleClick(QPoint widgetPoint, const GCCamera *cam, const XVector3D &direction) = 0;
  virtual void onMouseDrag(QPoint lastWidgetPoint, QPoint widgetPoint, const GCCamera *cam, const XVector3D &lastDirection, const XVector3D &direction) = 0;
  virtual void onMouseRelease(QPoint widgetPoint, const GCCamera *cam, const XVector3D &direction) = 0;
  };
  
class GRAPHICSCORE_EXPORT GCVisualCompoundManipulator : public GCVisualManipulator
  {
  S_ABSTRACT_PROPERTY_CONTAINER(GCVisualCompoundManipulator, GCVisualManipulator, 0)

public:
  GCVisualCompoundManipulator();
  
  SPropertyArray childManipulators;
  
  virtual bool hitTest(
    const QPoint &widgetSpacePoint,
    const GCCamera *camera,
    const XVector3D &clickDirection, // in world space
    float *distance,
    GCVisualManipulator **clicked);

  virtual void render(const GCCamera *camera, XRenderer *);

  virtual void onMouseClick(QPoint widgetPoint, const GCCamera *cam, const XVector3D &direction);
  virtual void onMouseDoubleClick(QPoint widgetPoint, const GCCamera *cam, const XVector3D &direction);
  virtual void onMouseDrag(QPoint lastWidgetPoint, QPoint widgetPoint, const GCCamera *cam, const XVector3D &lastDirection, const XVector3D &direction);
  virtual void onMouseRelease(QPoint widgetPoint, const GCCamera *cam, const XVector3D &direction);
  };


class GRAPHICSCORE_EXPORT GCVisualDragManipulator : public GCVisualManipulator
  {
  S_ABSTRACT_PROPERTY_CONTAINER(GCVisualDragManipulator, GCVisualManipulator, 0)

public:
  GCVisualDragManipulator();
  
  virtual void onDrag(
    const QPoint &oldWidgetSpacePoint,
    const QPoint &widgetSpacePoint,
    const XVector3D &cameraPosition,
    const XVector3D &oldClickDirection, // in world space
    const XVector3D &clickDirection) = 0; // in world space

  virtual void onMouseClick(QPoint widgetPoint, const GCCamera *cam, const XVector3D &direction);
  virtual void onMouseDoubleClick(QPoint widgetPoint, const GCCamera *cam, const XVector3D &direction);
  virtual void onMouseDrag(QPoint lastWidgetPoint, QPoint widgetPoint, const GCCamera *cam, const XVector3D &lastDirection, const XVector3D &direction);
  virtual void onMouseRelease(QPoint widgetPoint, const GCCamera *cam, const XVector3D &direction);
  };

class GRAPHICSCORE_EXPORT GCVisualClickManipulator : public GCVisualManipulator
  {
  S_ABSTRACT_PROPERTY_CONTAINER(GCVisualClickManipulator, GCVisualManipulator, 0)

public:
  GCVisualClickManipulator();

  virtual void onClick() = 0;

  virtual void onMouseClick(QPoint widgetPoint, const GCCamera *cam, const XVector3D &direction);
  virtual void onMouseDoubleClick(QPoint widgetPoint, const GCCamera *cam, const XVector3D &direction);
  virtual void onMouseDrag(QPoint lastWidgetPoint, QPoint widgetPoint, const GCCamera *cam, const XVector3D &lastDirection, const XVector3D &direction);
  virtual void onMouseRelease(QPoint widgetPoint, const GCCamera *cam, const XVector3D &direction);
  };

class GRAPHICSCORE_EXPORT GCLinearDragManipulator : public GCVisualDragManipulator
  {
  S_ABSTRACT_PROPERTY_CONTAINER(GCLinearDragManipulator, GCVisualDragManipulator, 0)

public:
  GCLinearDragManipulator();

  enum LockMode
    {
    Free,
    Planar,
    Linear
    };

  EnumProperty lockMode;
  Vector3DProperty lockDirection; // normal for planar, direction for linear

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
