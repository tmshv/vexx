#include "XQtWrappers.h"
#include "XScriptEngine.h"
#include "XInterface.h"

namespace XQtWrappers
{

template <typename T> void setupBindings(XInterface<T> *templ);

template <typename T> struct Binder
  {
  static void init(XScriptEngine *engine, const char *name)
    {
    XInterface<T> *templ = XInterface<T>::create(name);
    setupBindings<T>(templ);
    templ->seal();
    engine->addInterface(templ);
    }
  };

template <typename T> void setupBindings(XInterface<T> *templ)
  {
  (void)templ;
  }

template <> void setupBindings<QPointF>(XInterface<QPointF> *templ)
  {
  templ->addProperty<qreal, qreal, &QPointF::x, &QPointF::setX>("x");
  templ->addProperty<qreal, qreal, &QPointF::y, &QPointF::setY>("y");
  }

template <> void setupBindings<QPoint>(XInterface<QPoint> *templ)
  {
  templ->addProperty<int, int, &QPoint::x, &QPoint::setX>("x");
  templ->addProperty<int, int, &QPoint::y, &QPoint::setY>("y");
  }

template <> void setupBindings<QSizeF>(XInterface<QSizeF> *templ)
  {
  templ->addProperty<qreal, qreal, &QSizeF::width, &QSizeF::setWidth>("width");
  templ->addProperty<qreal, qreal, &QSizeF::height, &QSizeF::setHeight>("height");
  }

template <> void setupBindings<QSize>(XInterface<QSize> *templ)
  {
  templ->addProperty<int, int, &QSize::width, &QSize::setWidth>("width");
  templ->addProperty<int, int, &QSize::height, &QSize::setHeight>("height");
  }

template <> void setupBindings<QRectF>(XInterface<QRectF> *templ)
  {
  templ->addProperty<qreal, qreal, &QRectF::left, &QRectF::setLeft>("left");
  templ->addProperty<qreal, qreal, &QRectF::right, &QRectF::setRight>("right");
  templ->addProperty<qreal, qreal, &QRectF::top, &QRectF::setTop>("top");
  templ->addProperty<qreal, qreal, &QRectF::bottom, &QRectF::setBottom>("bottom");
  templ->addProperty<QPointF, const QPointF &, &QRectF::topLeft, &QRectF::setTopLeft>("topLeft");
  }
template <> void setupBindings<QRect>(XInterface<QRect> *templ)
  {
  templ->addProperty<int, int, &QRect::left, &QRect::setLeft>("left");
  templ->addProperty<int, int, &QRect::right, &QRect::setRight>("right");
  templ->addProperty<int, int, &QRect::top, &QRect::setTop>("top");
  templ->addProperty<int, int, &QRect::bottom, &QRect::setBottom>("bottom");
  templ->addProperty<QPoint, const QPoint &, &QRect::topLeft, &QRect::setTopLeft>("topLeft");
  }

void initiate(XScriptEngine *eng)
  {
#define BIND(t) Binder<t>::init(eng, #t)
  BIND(QPointF);
  BIND(QRectF);
  BIND(QPoint);
  BIND(QRect);
  BIND(QUrl);
  BIND(QBrush);
  BIND(QPen);
  BIND(QLocale);
  BIND(QSizeF);
  BIND(QSize);
  BIND(QFont);
  BIND(QIcon);
  BIND(QCursor);
  BIND(QSizePolicy);
  BIND(QPalette);
  BIND(QRegion);
#undef BIND
  }
}
