#ifndef UISURFACE_H
#define UISURFACE_H

#include "UIGlobal.h"
#include "XProperty"
#include "QString"

class QWidget;
class UISurfacePrivate;

class UI_EXPORT UISurface
  {
public:
  enum SurfaceType
    {
    Dock,
    PropertiesPage
    };

properties:
  XROProperty(QString, name);
  XROProperty(QWidget *, widget);
  XROProperty(SurfaceType, type);
  XProperty(UISurfacePrivate *, privateData, setPrivateData);

public:
  UISurface(QString name, QWidget *widget, SurfaceType type);
  virtual ~UISurface();
  };

#endif // UISURFACE_H
