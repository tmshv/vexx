#ifndef INEPROPERTIES_H
#define INEPROPERTIES_H

#include "UISurface.h"
#include "QObject"

class QListWidget;
class InePlugin;

class IneProperties : public QObject, public UISurface
  {
  Q_OBJECT

properties:
  XROProperty(InePlugin *, plugin);

public:
  IneProperties(InePlugin *plugin);

private slots:
  void refresh();

private:
  QListWidget *_list;
  };

#endif // INEPROPERTIES_H
