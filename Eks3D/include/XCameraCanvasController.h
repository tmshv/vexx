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
      Zoom = 1,
      Track = 2,
      Dolly = 4,
      Pan = 8,
      AllMovementTypes = Zoom | Track | Dolly | Pan
      };
    typedef XFlags<MovementType> MovementFlags;

    virtual ~CameraInterface() { }
    virtual MovementFlags supportedMovements() const { return AllMovementTypes; }
    virtual void zoom(float factor, float x, float y) { };
    virtual void track(float x, float y) { };
    virtual void dolly(float x, float y) { };
    virtual void pan(float x, float y) { };
    };

XProperties:
  XProperty(CameraInterface *, camera, setCamera);

public:
  XCameraCanvasController(CameraInterface *cam, XAbstractCanvas *canvas);

  virtual UsedFlags mouseEvent(MouseEventType type,
                          QPoint point,
                          Qt::MouseButton triggerButton,
                          Qt::MouseButtons buttonsDown,
                          Qt::KeyboardModifiers modifiers,
                          int orientation);
  };

#endif // XCAMERACANVASCONTROLLER_H
