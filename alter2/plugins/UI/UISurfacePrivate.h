#ifndef UISURFACEPRIVATE_H
#define UISURFACEPRIVATE_H

#include "XProperty"

class QDockWidget;
class UISurface;
class UIPlugin;

class UISurfacePrivate
  {
public:
  UISurfacePrivate();
  ~UISurfacePrivate();

properties:
  XProperty(QDockWidget *, dock, setDock);
  XProperty(UISurface *, surface, setSurface);
  XProperty(UIPlugin *, plugin, setPlugin);
  };

#endif // UISURFACEPRIVATE_H
