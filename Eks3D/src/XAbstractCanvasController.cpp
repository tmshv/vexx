#include "XAbstractCanvasController.h"

XAbstractCanvasController::XAbstractCanvasController(XAbstractCanvas *canvas) : _canvas(canvas)
  {
  }


XAbstractCanvasController::UsedFlags XAbstractCanvasController::triggerMouseEvent(MouseEventType type,
                       QPoint point,
                       Qt::MouseButton triggerButton,
                       Qt::MouseButtons buttonsDown,
                       Qt::KeyboardModifiers modifiers)
  {
  UsedFlags ret = mouseEvent(type, point, triggerButton, buttonsDown, modifiers);
  _lastKnownMousePosition = point;
  return ret;
  }



XAbstractCanvasController::UsedFlags XAbstractCanvasController::triggerWheelEvent(int delta,
                       Qt::Orientation orientation,
                       QPoint point,
                       Qt::MouseButtons buttonsDown,
                       Qt::KeyboardModifiers modifiers)
  {
  UsedFlags ret = wheelEvent(delta, orientation, point, buttonsDown, modifiers);
  _lastKnownMousePosition = point;
  return ret;
  }
