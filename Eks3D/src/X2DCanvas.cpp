#include "X2DCanvas.h"
#include "QPainter"
#include "QPaintEvent"
#include "QApplication"
#include "XAbstractCanvasController.h"

X2DCanvas::X2DCanvas(QWidget *parent) : QWidget(parent), _backgroundColour(QApplication::palette().color(QPalette::Window)), _currentPainter(0)
  {
  }

void X2DCanvas::paintEvent(QPaintEvent *event)
  {
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing, antiAliasingEnabled());
  _currentPainter = &painter;
  _region = event->rect();

  painter.fillRect(_region, _backgroundColour);

  paint();

  _currentPainter = 0;
  }

void X2DCanvas::update(XAbstractRenderModel::UpdateMode c)
  {
  XAbstractCanvas::update(c);
  QWidget::update();
  }

XSimple2DCanvasController::XSimple2DCanvasController(X2DCanvas *canvas) : XAbstractCanvasController(canvas)
  {
  }

int XSimple2DCanvasController::mouseEvent(MouseEventType type,
                        QPoint point,
                        Qt::MouseButton triggerButton,
                        Qt::MouseButtons buttonsDown,
                        Qt::KeyboardModifiers modifiers)
  {
  return NotUsed;
  }
