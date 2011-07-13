#include "X3DCanvas.h"

X3DCanvas::X3DCanvas(QWidget *parent) : QGLWidget(parent)
  {
  }

void X3DCanvas::paintGL()
  {
  paint();
  }

void X3DCanvas::update(XAbstractRenderModel::UpdateMode c)
  {
  XAbstractCanvas::update(c);
  QGLWidget::updateGL();
  }
