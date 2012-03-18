#ifndef TESTOBJECT_H
#define TESTOBJECT_H

#include "QObject"
#include "QDebug"

class TestObject : public QObject
  {
  Q_OBJECT

public:
  TestObject();

  Q_INVOKABLE QObject *thing(int a, float b)
    {
    qDebug() << "YAY" << a << b;
    return this;
    }
  };

#endif // TESTOBJECT_H
