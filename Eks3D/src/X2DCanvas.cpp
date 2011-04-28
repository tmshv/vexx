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

void X2DCanvas::mouseDoubleClickEvent(QMouseEvent *event)
  {
  if(controller())
    {
    int result = controller()->mouseEvent(XAbstractCanvasController::DoubleClick,
                                                  event->pos(),
                                                  event->button(),
                                                  event->buttons(),
                                                  event->modifiers());

    if((result&XAbstractCanvasController::Used) != false)
      {
      event->accept();
      }
    if((result&XAbstractCanvasController::NeedsUpdate) != false)
      {
      QWidget::update();
      }
    return;
    }

  event->ignore();
  }

void X2DCanvas::mouseMoveEvent(QMouseEvent *event)
  {
  if(controller())
    {
    int result = controller()->mouseEvent(XAbstractCanvasController::Move,
                                                  event->pos(),
                                                  event->button(),
                                                  event->buttons(),
                                                  event->modifiers());

    if((result&XAbstractCanvasController::Used) != false)
      {
      event->accept();
      }
    if((result&XAbstractCanvasController::NeedsUpdate) != false)
      {
      QWidget::update();
      }
    return;
    }

  event->ignore();
  }

void X2DCanvas::mousePressEvent(QMouseEvent *event)
  {
  if(controller())
    {
    int result = controller()->mouseEvent(XAbstractCanvasController::Press,
                                                  event->pos(),
                                                  event->button(),
                                                  event->buttons(),
                                                  event->modifiers());

    if((result&XAbstractCanvasController::Used) != false)
      {
      event->accept();
      }
    if((result&XAbstractCanvasController::NeedsUpdate) != false)
      {
      QWidget::update();
      }
    return;
    }

  event->ignore();
  }

void X2DCanvas::mouseReleaseEvent(QMouseEvent *event)
  {
  if(controller())
    {
    int result = controller()->mouseEvent(XAbstractCanvasController::Release,
                                                  event->pos(),
                                                  event->button(),
                                                  event->buttons(),
                                                  event->modifiers());

    if((result&XAbstractCanvasController::Used) != false)
      {
      event->accept();
      }
    if((result&XAbstractCanvasController::NeedsUpdate) != false)
      {
      QWidget::update();
      }
    return;
    }

  event->ignore();
  }

void X2DCanvas::update(XAbstractRenderModel::UpdateMode c)
  {
  XAbstractCanvas::update(c);
  QWidget::update();
  }
