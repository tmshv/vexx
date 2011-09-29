#ifndef UIEVENTLOGGERSURFACE_H
#define UIEVENTLOGGERSURFACE_H

#include "XProperty"
#include "UISurface.h"
#include "QWidget"

class EventLoggerWidget : public QWidget
  {
XProperties:
  XProperty(float, millisecondToPixelsScaleFactor, millisetSecondToPixelsScaleFactor);

public:
  EventLoggerWidget(QWidget *parent = 0);

  void onEventsLogged();
  void paintEvent(QPaintEvent *);
  QSize sizeHint() const;

  };

class UIEventLoggerSurface : public UISurface
  {
public:
  UIEventLoggerSurface();

private:
  EventLoggerWidget *_logger;
  };

#endif // UIEVENTLOGGERSURFACE_H
