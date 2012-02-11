#ifndef SYINTERFACE_H
#define SYINTERFACE_H

#include "QObject"
#include "syviewer.h"

class SyInterface : public QObject
  {
  Q_OBJECT

public:
  SyInterface();

  Q_INVOKABLE QObject *createViewer();
  };

#endif // SYINTERFACE_H
