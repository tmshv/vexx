#include "XCameraCanvasController.h"

XCameraCanvasController::XCameraCanvasController(CameraInterface *cam, XAbstractCanvas *canvas)
    : XAbstractCanvasController(canvas), _camera(cam)
  {
  }


XCameraCanvasController::UsedFlags XCameraCanvasController::mouseEvent(MouseEventType type,
                        QPoint point,
                        Qt::MouseButton triggerButton,
                        Qt::MouseButtons buttonsDown,
                        Qt::KeyboardModifiers modifiers,
                        int orientation)
  {
  if(!_camera)
    {
    return NotUsed;
    }

  CameraInterface::MovementFlags supported = _camera->supportedMovements();

  if(supported.hasFlag(CameraInterface::Zoom) && type == Wheel)
    {
    QPoint delta = lastKnownMousePosition() - point;
    float length = 1.0f + 0.0001f * sqrt(delta.x()*delta.x() + delta.y()*delta.y());

    _camera->zoom(length, point.x(), point.y());
    return Used|NeedsUpdate;
    }

  return NotUsed;
  }
