#include "XCameraCanvasController.h"
#include "QDebug"

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
    if(supported.hasFlag(CameraInterface::Track) &&
       ((triggerButton == Qt::MiddleButton && modifiers == Qt::NoModifier) || (triggerButton == Qt::LeftButton && modifiers == Qt::AltModifier)))
      {
      _current = CameraInterface::Track;
      return Used;
      }
    else if(supported.hasFlag(CameraInterface::Dolly) &&
       ((triggerButton == Qt::MiddleButton || buttonsDown == (Qt::LeftButton|Qt::RightButton)) && modifiers == (Qt::ShiftModifier|Qt::AltModifier)))
      {

      _current = CameraInterface::Pan;
      return Used;
      }
    else if(supported.hasFlag(CameraInterface::Dolly) &&
       ((triggerButton == Qt::MiddleButton || buttonsDown == (Qt::LeftButton|Qt::RightButton)) && modifiers == Qt::AltModifier))
      {

      _current = CameraInterface::Dolly;
      return Used;
      }
    else if(supported.hasFlag(CameraInterface::Zoom) &&
       (triggerButton == Qt::RightButton && modifiers == Qt::AltModifier))
      {
      _zoomCentre = point;
      _current = CameraInterface::Zoom;
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
    else if(_current == CameraInterface::Dolly)
      {
      _camera->dolly(delta.x(), delta.y());
      return Used|NeedsUpdate;
      }
    else if(_current == CameraInterface::Pan)
      {
      _camera->pan(delta.x(), delta.y());
      return Used|NeedsUpdate;
      }
    else if(_current == CameraInterface::Zoom)
      {
      QPoint delta = point - lastKnownMousePosition();
      float length = sqrt(delta.x()*delta.x() + delta.y()*delta.y());
      if(delta.y() < 0.0f)
        {
        length *= -1.0f;
        }
      length *= 0.01;
      length += 1.0f;

      _camera->zoom(length, _zoomCentre.x(), _zoomCentre.y());
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
