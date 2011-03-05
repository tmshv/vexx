#include "UISurfacePrivate.h"
#include "UIPlugin.h"

UISurfacePrivate::UISurfacePrivate() : _surface(0), _plugin(0)
  {
  }

UISurfacePrivate::~UISurfacePrivate()
  {
  if(_plugin)
    {
    _plugin->removeSurface(_surface);
    }
  }
