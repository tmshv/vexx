#include "XCameraCanvasController.h"

XCameraCanvasController::XCameraCanvasController(CameraInterface *cam, XAbstractCanvas *canvas)
    : XAbstractCanvasController(canvas), _camera(cam), _current(CameraInterface::None)
  {
  }


XCameraCanvasController::UsedFlags XCameraCanvasController::mouseEvent(MouseEventType type,
                        QPoint point,
                        Qt::MouseButton triggerButton,
                        Qt::MouseButtons buttonsDown,
                        Qt::KeyboardModifiers modifiers)
  {
  if(!_camera)
    {
    return NotUsed;
    }

  CameraInterface::MovementFlags supported = _camera->supportedMovements();

  if(type == Press)
    {
    if(supported.hasFlag(CameraInterface::Track) && triggerButton == Qt::MiddleButton)
      {
      _current = CameraInterface::Track;
      return Used;
      }
    }
  else if(type == Move)
    {
    QPoint delta = point - lastKnownMousePosition();
    if(_current == CameraInterface::Track)
      {
      _camera->track(delta.x(), delta.y());
      return Used|NeedsUpdate;
      }
    }
  else if(type == Release)
    {
    if(_current != CameraInterface::None)
      {
      _current = CameraInterface::None;

      return Used;
      }
    }

  return NotUsed;
  }


XCameraCanvasController::UsedFlags XCameraCanvasController::wheelEvent(int delta,
                                                                       Qt::Orientation orientation,
                                                                       QPoint point,
                                                                       Qt::MouseButtons buttonsDown,
                                                                       Qt::KeyboardModifiers modifiers)
  {
  if(!_camera)
    {
    return NotUsed;
    }

  CameraInterface::MovementFlags supported = _camera->supportedMovements();

  if(supported.hasFlag(CameraInterface::Zoom))
    {
    float length = 1.0f + (delta * 0.002f);

    _camera->zoom(length, point.x(), point.y());
    return Used|NeedsUpdate;
    }

  return NotUsed;
  }
