#ifndef SCDECLARATIVESURFACE_H
#define SCDECLARATIVESURFACE_H

#include "QDeclarativeView"
#include "UISurface.h"
#include "QScriptValue"

class SDatabaseModel;

class ScDeclarativeWindow : public QDeclarativeView
  {
  Q_OBJECT

public:
  ScDeclarativeWindow(const QString &s, SDatabaseModel *model, const QVariantMap &);

  Q_INVOKABLE QVariantMap mapTo(QWidget *, const float x, const float y) const;
  Q_INVOKABLE void destroyWindow();

  Q_INVOKABLE void setPosition(int x, int y);
  Q_INVOKABLE QVariantMap position() const;

public slots:
  void focus();

signals:
  void emitRequest(const QString &, const QVariantList &);
  void focusLost();

protected:
  void focusOutEvent(QFocusEvent *event);
  };

class ScDeclarativeSurface : public ScDeclarativeWindow, public UISurface
  {
  Q_OBJECT

public:
  ScDeclarativeSurface(const QString &name, const QString &s, UISurface::SurfaceType type, SDatabaseModel *model, const QVariantMap &);
  };

#endif // SCDECLARATIVESURFACE_H
