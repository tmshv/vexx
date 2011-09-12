#ifndef SCIO_H
#define SCIO_H

#include "QObject"
#include "QFile"

class ScFile : public QFile
  {
  Q_OBJECT
public:

  ScFile(const QString& file="");
  Q_INVOKABLE bool open(const QString &strMode);
  Q_INVOKABLE void close();
  };


class ScIO : public QObject
  {
  Q_OBJECT

public:
  ScIO();

  Q_INVOKABLE QObject *createFile(const QString &path);
  };

#endif // SCIO_H
