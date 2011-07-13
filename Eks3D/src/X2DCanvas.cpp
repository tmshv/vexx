#include "X2DCanvas.h"
#include "QPainter"
#include "QPaintEvent"
#include "QApplication"
#include "XAbstractCanvasController.h"
#include "QDebug"

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

  painter.setTransform(_transform);

  paint();

  _currentPainter = 0;
  }

void X2DCanvas::update(XAbstractRenderModel::UpdateMode c)
  {
  XAbstractCanvas::update(c);
  QWidget::update();
  }

XSimple2DCanvasController::XSimple2DCanvasController(X2DCanvas *canvas) : XCameraCanvasController(this, canvas)
  {
  }

XSimple2DCanvasController::UsedFlags XSimple2DCanvasController::mouseEvent(MouseEventType type,
                        QPoint point,
                        Qt::MouseButton triggerButton,
                        Qt::MouseButtons buttonsDown,
                        Qt::KeyboardModifiers modifiers,
                        int orientation)
  {
  return XCameraCanvasController::mouseEvent(type, point, triggerButton, buttonsDown, modifiers, orientation);
  }

XSimple2DCanvasController::MovementFlags XSimple2DCanvasController::supportedMovements() const
  {
  return Track | Zoom;
  }

void XSimple2DCanvasController::zoom(float factor, float x, float y)
  {
  QTransform& transform = static_cast<X2DCanvas*>(canvas())->transform();

  //transform.translate(-x, -y);

  transform.scale(factor, factor);

  //transform.translate(x, y);
  }

void XSimple2DCanvasController::track(float x, float y)
  {
  qDebug() << "track" << x << y;
  }

void XSimple2DCanvasController::dolly(float x, float y)
  {
  qDebug() << "dolly" << x << y;
  }

void XSimple2DCanvasController::pan(float x, float y)
  {
  qDebug() << "pan" << x << y;
  }
