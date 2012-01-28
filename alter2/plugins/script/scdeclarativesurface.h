#ifndef SCDECLARATIVESURFACE_H
#define SCDECLARATIVESURFACE_H

#include "QDeclarativeView"
#include "UISurface.h"

class SDatabaseModel;

class ScDeclarativeSurface : public QDeclarativeView, public UISurface
  {
  Q_OBJECT

public:
  ScDeclarativeSurface(const QString &name, const QString &s, UISurface::SurfaceType type, SDatabaseModel *model, const QVariantMap &);

signals:
  void send(const QString &, const QVariantList &);
  void recieve(const QString &, const QVariantList &);
  };

#endif // SCDECLARATIVESURFACE_H
