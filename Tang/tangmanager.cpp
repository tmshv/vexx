#include "tangmanager.h"
#include "viewport.h"
#include "aplugin.h"

TangManager::TangManager(ACore *core) : QObject(core), _core(core)
  {
  }

QWidget *TangManager::createViewport()
  {
  APlugin<SPlugin> db(*core(), "db");
  if(!db.isValid())
    {
    xAssertFail();
    return 0;
    }

  return new Viewport(*db);
  }
