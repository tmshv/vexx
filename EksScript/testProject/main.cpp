#include "XInterface.h"
#include "XSignatureHelpers.h"
#include "XScriptObject.h"
#include "XEngine.h"
#include "XContext.h"
#include "XScriptSource.h"
#include "XScriptConstructors.h"
#include "XFunctions.h"

#include "QRectF"


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

template <> const XInterfaceBase* findInterface<Inheritable>(const Inheritable *toWrap)
  {
  if(dynamic_cast<const Inheritor*>(toWrap))
    {
    return XInterface<Inheritor>::lookup();
    }
  return XInterface<Inheritable>::lookup();
  }

template <> const XInterfaceBase* findInterface<Inheritor>(const Inheritor *toWrap)
  {
  return findInterface<Inheritable>(toWrap);
  }

X_SCRIPTABLE_TYPE_NOT_COPYABLE(SomeClass)
X_SCRIPTABLE_TYPE_COPYABLE(QRectF)
X_SCRIPTABLE_TYPE_COPYABLE(QPointF)
X_SCRIPTABLE_TYPE(Inheritable)
X_SCRIPTABLE_TYPE_INHERITS(Inheritor, Inheritable)


int SomeClass::a = 0;
int SomeClass::otherStatic = 0;

int main(int, char*[])
{
  XEngine engine;

  // build the template
  XInterface<SomeClass> *someTempl = XInterface<SomeClass>::create("SomeClass");
  someTempl->addProperty<const QRectF &, const QRectF &, &SomeClass::getNonStatic, &SomeClass::setNonStatic>("nonStatic");
  someTempl->addAccessProperty<Inheritable *, &SomeClass::inh>("thing");
  someTempl->seal();
  //someTempl.addPropertyMap<&SomeClass::getOther, &SomeClass::setOther, SomeClass::getOtherStatic, SomeClass::setOtherStatic>();

  XInterface<QPointF> *ptTempl = XInterface<QPointF>::create("Point");
  ptTempl->addProperty<qreal, qreal, &QPointF::x, &QPointF::setX>("x");
  ptTempl->addProperty<qreal, qreal, &QPointF::y, &QPointF::setY>("y");
  ptTempl->seal();

  XInterface<QRectF> *rectTempl = XInterface<QRectF>::create("Rect");
  rectTempl->addProperty<qreal, qreal, &QRectF::left, &QRectF::setLeft>("left");
  rectTempl->addProperty<qreal, qreal, &QRectF::right, &QRectF::setRight>("right");
  rectTempl->addProperty<qreal, qreal, &QRectF::top, &QRectF::setTop>("top");
  rectTempl->addProperty<qreal, qreal, &QRectF::bottom, &QRectF::setBottom>("bottom");
  rectTempl->addProperty<QPointF, const QPointF &, &QRectF::topLeft, &QRectF::setTopLeft>("topLeft");
  rectTempl->seal();

  XInterface<Inheritable> *aTempl = XInterface<Inheritable>::create("Inheritable");
  aTempl->addProperty<int, int, &Inheritable::getA, &Inheritable::setA>("a");
  aTempl->seal();

  XInterface<Inheritor> *bTempl = XInterface<Inheritor>::create("Inheritor", aTempl);
  bTempl->addProperty<int, int, &Inheritor::getB, &Inheritor::setB>("b");
  bTempl->seal();


  // Create a new context.
  XContext c(&engine);
  c.addInterface(someTempl);
  c.addInterface(rectTempl);
  c.addInterface(ptTempl);
  c.addInterface(aTempl);
  c.addInterface(bTempl);

  SomeClass *m = NULL;
  XScriptObject obj = XScriptObject::newInstance(someTempl);
  m = obj.castTo<SomeClass>();

  c.set("someClass", obj);

  XScriptSource script(
    "var a = someClass.thing;"
    "var r = new Rect();"
    "r.top = 5;"
    "r.left = 61;"
    "'Hello' + someClass.nonStatic + \" \" + someClass.nonStatic.topLeft.x + \" \" + someClass.nonStatic.topLeft.y + \" \" + a + \" \" + a.a + \" \" + a.b;"
    );

  bool err = false;
  XScriptValue result = script.run(&err);
  xAssert(!err);
  qDebug() << result.toString();

  return 0;
}
