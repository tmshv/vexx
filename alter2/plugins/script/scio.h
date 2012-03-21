#if 0 //ndef SCIO_H
#define SCIO_H

#include "QObject"
#include "QFile"

class QScriptValue;

class ScFile : public QFile
  {
  Q_OBJECT
public:

  ScFile(const QString& file="");
  Q_INVOKABLE bool open(const QString &strMode);
  Q_INVOKABLE void close();
  Q_INVOKABLE void write(const QScriptValue &val);
  };


class ScIO : public QObject
  {
  Q_OBJECT

public:
  ScIO();

  Q_INVOKABLE QObject *createFile(const QString &path);
  };

#endif // SCIO_H
