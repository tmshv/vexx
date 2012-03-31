#ifndef ADEBUGINTERFACE_H
#define ADEBUGINTERFACE_H

#include "aglobal.h"
#include "XProperty"
#include "QString"
#include "QObject"

class QIODevice;
class QTcpSocket;

class ALTER_EXPORT ADebugInterface : public QObject
  {
  Q_OBJECT
  XROProperty(QString, type)

public:
  ADebugInterface(const QString &type);
  ~ADebugInterface();

  QIODevice *device();
  bool readyForData() const;

signals:
  void readyRead();

private:
  QTcpSocket *_socket;
  };

#endif // ADEBUGINTERFACE_H
