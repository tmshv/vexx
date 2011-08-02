#include "UISurface.h"
#include "UISurfacePrivate.h"
#include "QDebug"

UISurface::UISurface(QString name, QWidget *widget, SurfaceType t) : _name(name), _widget(widget), _type(t), _privateData(0)
  {
  }

UISurface::~UISurface()
  {
  delete _privateData;
  }
