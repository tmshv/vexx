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

  const xint32 drawXOffset = SafeBorder;
  const xint32 drawYOffset = SafeBorder;

  QRect rect = pE->rect();
  p.setClipRect(rect);

  p.fillRect(rect, Qt::black);

  // available time, should be stretched form 0 to width
  XTime minAvailableTime, maxAvailableTime;
  XEventManager::totalAvailableTime(minAvailableTime, maxAvailableTime);
  XTime availableTime = maxAvailableTime - minAvailableTime;

  rect.moveBottomLeft(QPoint(drawXOffset, 0));

  float minRatio = 0.0f;//(float)rect.x() / (float)width();
  float maxRatio = 1.0f;//(float)rect.right() / (float)width();

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

        if(!inside)
          {
          poly << QPointF(millisecondX - (_millisecondToPixelsScaleFactor*SafeBorder), unscaledHeight);
          }

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

  p.setRenderHint(QPainter::Antialiasing, true);
  float drawHeight = height() - 2*SafeBorder;
  for(xsize i = 0, s=polys.size(); i<s; ++i)
    {
    QPolygonF &poly = polys[i];

    // scale to fit in widget
    float lastX = 0;
    for(xsize j=0, t=poly.size(); j<t; ++j)
      {
      QPointF &pt = poly[j];

      float y = pt.y();

      y -= minUnscaledHeight;
      y /= maxUnscaledHeight - minUnscaledHeight;
      y *= drawHeight;
      y = drawYOffset + drawHeight - y; // flip to y points upward
      xAssert(y >= drawYOffset);
      xAssert(y <= drawHeight + drawYOffset);

      pt.setY(y);

      float x = pt.x();
      x *= _millisecondToPixelsScaleFactor; // convert x from milliseconds to pixels
      if(j == 0)
        {
        x = 0;
        }

      pt.setX(x);
      lastX = x;
      }

    QPointF pt = poly.back();
    pt.setX(width());
    poly << pt;

    poly << QPointF(width(), height());
    poly << QPointF(0, height());

    QPainterPath path;
    path.addPolygon(poly);

    p.setPen(Qt::white);

    p.drawPath(path);

    for(xsize j=1, t=(poly.size()-3); j<t; ++j)
      {
      QPointF &pt = poly[j];

      QColor col(255, 255, 255, 128);
      p.setPen(col);
      p.drawEllipse(pt, 2, 2);
      }
    }
  }

void EventLoggerWidget::onEventsLogged()
  {
  updateGeometry();
  update(rect());
  }

QSize EventLoggerWidget::sizeHint() const
  {
  XTime minTime, maxTime;
  XEventManager::totalAvailableTime(minTime, maxTime);

  XTime availableTime = maxTime - minTime;
  return QSize(_millisecondToPixelsScaleFactor * availableTime.milliseconds() + (SafeBorder * 2), 1);
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
