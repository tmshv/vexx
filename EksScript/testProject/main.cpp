#include "v8.h"
#include "QMetaType"
#include "QVariant"
#include "QHash"
#include "cvv8/v8-convert.hpp"
#include "QRectF"
#include "XMacroHelpers"
#include "XInterface.h"
#include "XInterfaceObject.h"
#include "XEngine.h"
#include "XContext.h"
#include "XScript.h"

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
};
int SomeClass::a = 0;
int SomeClass::otherStatic = 0;

//--------------------------------------------------------------------------
X_SCRIPTABLE_TYPE_NOT_COPYABLE(SomeClass)
X_IMPLEMENT_SCRIPTABLE_TYPE_NOT_COPYABLE(SomeClass, "SomeClass")

//--------------------------------------------------------------------------
X_SCRIPTABLE_TYPE_COPYABLE(QRectF)
X_IMPLEMENT_SCRIPTABLE_TYPE_COPYABLE(QRectF, "Rect")

//--------------------------------------------------------------------------
X_SCRIPTABLE_TYPE_COPYABLE(QPointF)
X_IMPLEMENT_SCRIPTABLE_TYPE_COPYABLE(QPointF, "Point")

int main(int, char*[])
{
  XEngine engine;

  // build the template
  XInterface<SomeClass> someTempl;
  someTempl.addProperty<const QRectF &, const QRectF &, &SomeClass::getNonStatic, &SomeClass::setNonStatic>("nonStatic");
  //someTempl.addPropertyMap<&SomeClass::getOther, &SomeClass::setOther, SomeClass::getOtherStatic, SomeClass::setOtherStatic>();

  XInterface<QPointF> ptTempl;
  ptTempl.addProperty<qreal, qreal, &QPointF::x, &QPointF::setX>("x");
  ptTempl.addProperty<qreal, qreal, &QPointF::y, &QPointF::setY>("y");

  XInterface<QRectF> rectTempl;
  rectTempl.addProperty<qreal, qreal, &QRectF::left, &QRectF::setLeft>("left");
  rectTempl.addProperty<qreal, qreal, &QRectF::right, &QRectF::setRight>("right");
  rectTempl.addProperty<qreal, qreal, &QRectF::top, &QRectF::setTop>("top");
  rectTempl.addProperty<qreal, qreal, &QRectF::bottom, &QRectF::setBottom>("bottom");
  rectTempl.addProperty<QPointF, const QPointF &, &QRectF::topLeft, &QRectF::setTopLeft>("topLeft");

  // Create a new context.
  XContext c(&engine);
  c.addInterface(someTempl);
  c.addInterface(rectTempl);
  c.addInterface(ptTempl);

  SomeClass *m = NULL;
  XInterfaceObject obj = XInterfaceObject::newInstance(&someTempl, &m);

  c.set("someClass", obj);

  XScript script("var r = new Rect();"
                "r.top = 5;"
                "r.left = 61;"
                "someClass.nonStatic = r;"
                "'Hello' + someClass.nonStatic + \" \" + someClass.nonStatic.topLeft.x + \" \" + someClass.nonStatic.topLeft.y;");

  script.run();

  v8::V8::LowMemoryNotification();

  return 0;
}
