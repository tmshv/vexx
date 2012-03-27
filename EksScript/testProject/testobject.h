#ifndef TESTOBJECT_H
#define TESTOBJECT_H

#include "QObject"
#include "QDebug"
#include "QRectF"
#include "XInterface.h"


class Inheritable
  {
public:
  virtual ~Inheritable(){}
  int getA() const
    {
    return 3;
    }

  void setA(int)
    {
    }
  };

class Inheritor : public Inheritable
  {
public:
  int getB() const
    {
    return 4;
    }

  void setB(int )
    {
    }
  };

class SomeClass
{
public:
  SomeClass() : v()
  {
  }
  virtual ~SomeClass() {}

  virtual const QRectF &getNonStatic() const
  {
    return v;
  }

  virtual void setNonStatic(const QRectF &inA)
  {
    v = inA;
  }

  Inheritable *inh()
    {
    return &_inh;
    }

  QVariant getOther(const char *name)
  {
    if(strcmp(name, "other") == 0)
    {
      return other;
    }
    return QVariant();
  }

  virtual bool setOther(const char *name, const QVariant &inA)
  {
    if(strcmp(name, "other") == 0)
    {
      other = inA.toInt();
      return true;
    }
    return false;
  }

  static QVariant getOtherStatic(const char *name)
  {
    if(strcmp(name, "otherStatic") == 0)
    {
      return otherStatic;
    }
    return QVariant();
  }

  static bool setOtherStatic(const char *name, const QVariant &inA)
  {
    if(strcmp(name, "otherStatic") == 0)
    {
      otherStatic = inA.toInt();
      return true;
    }
    return false;
  }

  static int getA()
  {
    return a;
  }

  static void setA(const int &inA)
  {
    a = inA;
  }

  int nonStatic;
  int other;
  static int a;
  static int otherStatic;
  QRectF v;
  Inheritor _inh;
};

X_SCRIPTABLE_TYPE(SomeClass)
X_SCRIPTABLE_TYPE(Inheritable)
X_SCRIPTABLE_TYPE_INHERITS(Inheritor, Inheritable)

template <> inline const XInterfaceBase* findInterface<Inheritable>(const Inheritable *toWrap)
  {
  if(dynamic_cast<const Inheritor*>(toWrap))
    {
    return XInterface<Inheritor>::lookup();
    }
  return XInterface<Inheritable>::lookup();
  }

template <> inline const XInterfaceBase* findInterface<Inheritor>(const Inheritor *toWrap)
  {
  return findInterface<Inheritable>(toWrap);
  }

class TestObject : public QObject
  {
  Q_OBJECT

  SomeClass _a;

public:
  TestObject();

  Q_INVOKABLE SomeClass *thing(TestObject *b)
    {
    qDebug() << "YAY" << b;
    return &_a;
    }
  };

#endif // TESTOBJECT_H
