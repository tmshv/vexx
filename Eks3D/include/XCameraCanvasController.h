#ifndef XCAMERACANVASCONTROLLER_H
#define XCAMERACANVASCONTROLLER_H

#include "XProperty"
#include "XAbstractCanvasController.h"

class EKS3D_EXPORT XCameraCanvasController : public XAbstractCanvasController
  {
public:
  class CameraInterface
    {
  public:
    enum MovementType
      {
      None = 0,
      Zoom = 1,
      Track = 2,
      Dolly = 4,
      Pan = 8,
      AllMovementTypes = Zoom | Track | Dolly | Pan
      };
    typedef XFlags<MovementType> MovementFlags;

    virtual ~CameraInterface() { }
    virtual MovementFlags supportedMovements() const { return AllMovementTypes; }
    virtual void zoom(float, float, float) { }
    virtual void track(float, float) { }
    virtual void dolly(float, float) { }
    virtual void pan(float, float) { }
    };

XProperties:
  XProperty(CameraInterface *, camera, setCamera);

public:
  XCameraCanvasController(CameraInterface *cam, XAbstractCanvas *canvas);

  virtual UsedFlags mouseEvent(MouseEventType type,
                          QPoint point,
                          Qt::MouseButton triggerButton,
                          Qt::MouseButtons buttonsDown,
                          Qt::KeyboardModifiers modifiers);

  virtual UsedFlags wheelEvent(int delta,
                               Qt::Orientation orientation,
                               QPoint point,
                               Qt::MouseButtons buttonsDown,
                               Qt::KeyboardModifiers modifiers);

private:
  CameraInterface::MovementType _current;
  QPoint _zoomCentre;
  };

#endif // XCAMERACANVASCONTROLLER_H
