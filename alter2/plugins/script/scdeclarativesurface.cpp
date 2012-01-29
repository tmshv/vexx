#include "scdeclarativesurface.h"
#include "sdatabasemodel.h"
#include "QDeclarativeContext"

ScDeclarativeWindow::ScDeclarativeWindow(const QString &s, SDatabaseModel *model, const QVariantMap &data)
  {
  QDeclarativeContext *ctx = rootContext();
  if(model)
    {
    ctx->setContextProperty("db", model);
    }

  ctx->setContextProperty("external", this);

  setSource(s);

  bool setWindowHint = false;
  Qt::WindowFlags flags;
  if(data["transparent"].toBool())
    {
    setAttribute(Qt::WA_TranslucentBackground);
    setStyleSheet("background:transparent;");
    setWindowHint = true;
    flags |= Qt::FramelessWindowHint;
    }

  if(data["tool"].toBool())
    {
    setWindowHint = true;
    flags |= Qt::Tool;
    }

  if(setWindowHint)
    {
    setWindowFlags(flags);
    }

  QString focus = data["focusPolicy"].toString();
  if(focus == "strong")
    {
    setFocusPolicy(Qt::StrongFocus);
    }

  /*QGraphicsObject *root = rootObject();
  foreach(const QString &s, data.keys())
    {
    root->setProperty(s.toLatin1().constData(), data.value(s));
    }*/
  }

QVariantMap ScDeclarativeWindow::mapTo(QWidget *w, const float x, const float y) const
  {
  QPoint g = QWidget::mapToGlobal(QPoint(x, y));

  if(w)
    {
    g = w->mapFromGlobal(g);
    }

  QVariantMap v;
  v["x"] = g.x();
  v["y"] = g.y();
  return v;
  }

void ScDeclarativeWindow::destroyWindow()
  {
  deleteLater();
  }

void ScDeclarativeWindow::setPosition(int x, int y)
  {
  move(x, y);
  }

void ScDeclarativeWindow::focus()
  {
  activateWindow();
  setFocus(Qt::MouseFocusReason);
  }

void ScDeclarativeWindow::focusOutEvent(QFocusEvent *)
  {
  emit focusLost();
  }

ScDeclarativeSurface::ScDeclarativeSurface(const QString &name,
                                           const QString &s,
                                           UISurface::SurfaceType type,
                                           SDatabaseModel *model,
                                           const QVariantMap &data)
    : ScDeclarativeWindow(s, model, data), UISurface(name, this, type)
  {
  setResizeMode(QDeclarativeView::SizeRootObjectToView);
  }
