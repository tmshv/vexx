#include "XInterface.h"
#include "XSignatureHelpers.h"
#include "XScriptObject.h"
#include "XEngine.h"
#include "XContext.h"
#include "XScriptSource.h"
#include "XScriptConstructors.h"
#include "XFunctions.h"
#include "XQObjectWrapper.h"

#include "QApplication"
#include "testobject.h"

int main(int a, char* b[])
{
  QApplication app(a, b);
  XEngine engine;

  qRegisterMetaType<SomeClass*>("SomeClass*");
  qRegisterMetaType<Inheritable*>("Inheritable*");
  qRegisterMetaType<Inheritor*>("Inheritor*");

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

  XInterface<Inheritor> *bTempl = XInterface<Inheritor>::createWithParent("Inheritor", aTempl);
  bTempl->addProperty<int, int, &Inheritor::getB, &Inheritor::setB>("b");
  bTempl->seal();


  // Create a new context.
  XContext c(&engine);
  XQObjectWrapper::initiate(&c);
  c.addInterface(someTempl);
  c.addInterface(rectTempl);
  c.addInterface(ptTempl);
  c.addInterface(aTempl);
  c.addInterface(bTempl);

  SomeClass *m = 0;
  XScriptObject obj = XScriptObject::newInstance(someTempl);
  m = obj.castTo<SomeClass>();


  TestObject file;
  XScriptValue jsFile = XScriptConvert::to<QObject*>(&file);

  c.set("file", jsFile);
  c.set("someClass", obj);

  XScriptSource script(
    /*"var a = someClass.thing;"*/
    "var r = new Rect();"
    "r.top = 5;"
    "r.left = 10;"
    "r.bottom = 100;"
    "r.right = 20;"
    /*"'Hello' + someClass.nonStatic + \" \" + someClass.nonStatic.topLeft.x + \" \" + someClass.nonStatic.topLeft.y + \" \" + a + \" \" + a.a + \" \" + a.b;"
    */
    "file.objectName = 'CAKE';"
    "'YO ' + file.thing(file);"
    );

  bool err = false;
  XScriptValue result = script.run(&err);
  xAssert(!err);
  qDebug() << result.toString();

  return 0;
}
