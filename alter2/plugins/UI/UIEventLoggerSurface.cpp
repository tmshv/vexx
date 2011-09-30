#include "UIEventLoggerSurface.h"
#include "QScrollArea"
#include "QVBoxLayout"
#include "QPushButton"
#include "QPainter"
#include "QPaintEvent"
#include "QDebug"

EventLoggerWidget::EventLoggerWidget(QWidget *parent) : QWidget(parent), _millisecondToPixelsScaleFactor(0.1f)
  {
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
  }

float EventLoggerWidget::typeToUnscaledHeight(int type, const void *d)
  {
  if(type == qMetaTypeId<xsize>())
    {
    xsize *s = (xsize*)d;
    return (float)*s;
    }

  xAssertFail();
  return 0.0f;
  }

void EventLoggerWidget::paintEvent(QPaintEvent *pE)
  {
  QPainter p(this);

  // available time, should be stretched form 0 to width
  XTime minAvailableTime, maxAvailableTime;
  XEventManager::totalAvailableTime(minAvailableTime, maxAvailableTime);
  XTime availableTime = maxAvailableTime - minAvailableTime;

  QRect rect = pE->rect();
  float minRatio = (float)rect.x() / (float)width();
  float maxRatio = (float)rect.right() / (float)width();

  XTime min = minAvailableTime + (availableTime * minRatio);
  XTime max = minAvailableTime + (availableTime * maxRatio);

  QVector <QPolygonF> polys;
  float minUnscaledHeight = FLT_MAX;
  float maxUnscaledHeight = -FLT_MAX;

  XEventLoggerInternal *e = XEventManager::firstEvent();
  while(e)
    {
    polys << QPolygonF();
    QPolygonF &poly = polys.back();
    bool inside = false;
    for(xsize i=0, s=e->used(); i<s; ++i)
      {
      const void *data = e->at(i);
      xAssert(data);
      const XTime *t = (const XTime *)data;

      if(inside || (*t > min && *t < max))
        {
        const void *tStoredData = e->dataAt(i);

        float millisecondX = (*t - min).milliseconds();
        float unscaledHeight = typeToUnscaledHeight(e->type, tStoredData);
        poly << QPointF(millisecondX, unscaledHeight);

        if(unscaledHeight > maxUnscaledHeight)
          {
          maxUnscaledHeight = unscaledHeight;
          }
        if(unscaledHeight < minUnscaledHeight)
          {
          minUnscaledHeight = unscaledHeight;
          }

        if(*t < max)
          {
          inside = true;
          }
        else
          {
          inside = false;
          }
        }
      }

    e = e->next;
    }

  // base at zero.
  minUnscaledHeight = qMin(0.0f, minUnscaledHeight);
  maxUnscaledHeight = qMax(0.0f, maxUnscaledHeight);

  // nothing to draw if at 0.0f;
  if(qFuzzyCompare(minUnscaledHeight, 0.0f) && qFuzzyCompare(maxUnscaledHeight, 0.0f))
    {
    return;
    }

  float drawHeight = height();
  for(xsize i = 0, s=polys.size(); i<s; ++i)
    {
    QPolygonF &poly = polys[i];

    // scale to fit in widget
    for(xsize j=0, t=poly.size(); j<t; ++j)
      {
      QPointF &pt = poly[j];

      float y = pt.y();

      y -= minUnscaledHeight;
      y *= drawHeight;
      y /= maxUnscaledHeight - minUnscaledHeight;
      y = drawHeight - y; // flip to y points upward

      pt.setY(y);

      float x = pt.x();
      x *= _millisecondToPixelsScaleFactor; // convert x from milliseconds to pixels

      pt.setX(x);

      qDebug() << pt;
      }

    QPainterPath path;
    path.addPolygon(poly);

    p.drawPath(path);
    }
  }

void EventLoggerWidget::onEventsLogged()
  {
  updateGeometry();
  update();
  }

QSize EventLoggerWidget::sizeHint() const
  {
  XTime minTime, maxTime;
  XEventManager::totalAvailableTime(minTime, maxTime);

  XTime availableTime = maxTime - minTime;
  return QSize(_millisecondToPixelsScaleFactor * availableTime.milliseconds(), 1);
  }

UIEventLoggerSurface::UIEventLoggerSurface()
    : UISurface("Event Logger", new QWidget(), UISurface::Dock)
  {
  QVBoxLayout *mainLayout = new QVBoxLayout(widget());

  _logger = new EventLoggerWidget();

  QScrollArea *scroller = new QScrollArea();
  scroller->setWidgetResizable(true);
  scroller->setWidget(_logger);
  mainLayout->addWidget(scroller);

  QPushButton *demoButton = new QPushButton();
  mainLayout->addWidget(demoButton);
  }
