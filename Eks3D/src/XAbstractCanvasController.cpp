#include "XAbstractCanvasController.h"

XAbstractCanvasController::XAbstractCanvasController(XAbstractCanvas *canvas) : _canvas(canvas)
  {
  }


XAbstractCanvasController::UsedFlags XAbstractCanvasController::triggerMouseEvent(MouseEventType type,
                       QPoint point,
                       Qt::MouseButton triggerButton,
                       Qt::MouseButtons buttonsDown,
                       Qt::KeyboardModifiers modifiers,
                       int orientation)
  {
  UsedFlags ret = mouseEvent(type, point, triggerButton, buttonsDown, modifiers, orientation);
  _lastKnownMousePosition = point;
  return ret;
  }
