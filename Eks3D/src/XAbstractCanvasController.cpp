#include "XAbstractCanvasController.h"

XAbstractCanvasController::XAbstractCanvasController(XAbstractCanvas *canvas) : _canvas(canvas)
  {
  }


int XAbstractCanvasController::triggerMouseEvent(MouseEventType type,
                       QPoint point,
                       Qt::MouseButton triggerButton,
                       Qt::MouseButtons buttonsDown,
                       Qt::KeyboardModifiers modifiers)
  {
  int ret = mouseEvent(type, point, triggerButton, buttonsDown, modifiers);
  _lastKnownMousePosition = point;
  return ret;
  }
