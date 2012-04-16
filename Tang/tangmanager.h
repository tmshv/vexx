#ifndef TANGMANAGER_H
#define TANGMANAGER_H

#include "XProperty"
#include "QObject"

class ACore;

class TangManager : public QObject
  {
  Q_OBJECT

XProperties:
  XROProperty(ACore*, core);

public:
  TangManager(ACore *core);

  Q_INVOKABLE QWidget *createViewport();
  };

#endif // TANGMANAGER_H
