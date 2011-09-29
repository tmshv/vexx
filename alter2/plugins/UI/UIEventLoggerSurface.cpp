#include "UIEventLoggerSurface.h"
#include "QScrollArea"
#include "QVBoxLayout"
#include "QPushButton"
#include "QPainter"
#include "QPaintEvent"
#include "QDebug"

EventLoggerWidget::EventLoggerWidget(QWidget *parent) : QWidget(parent), _millisecondToPixelsScaleFactor(0.1)
  {
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
  }

void EventLoggerWidget::paintEvent(QPaintEvent *e)
  {
  QPainter p(this);

  XTime minTime, maxTime;
  XEventManager::totalAvailableTime(minTime, maxTime);

  XTime availableTime = maxTime - minTime;
  xuint32 width = _millisecondToPixelsScaleFactor * availableTime.milliseconds();

  qDebug() << width;
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
