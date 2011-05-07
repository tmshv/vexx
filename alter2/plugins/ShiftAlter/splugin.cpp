#include "splugin.h"
#include "sglobal.h"
#include "sprocessmanager.h"
#include "QThread"

ALTER_PLUGIN(SPlugin);

SPlugin::SPlugin()
  {
  }

SAppDatabase &SPlugin::db()
  {
  return _db;
  }

const SAppDatabase &SPlugin::db() const
  {
  return _db;
  }

void SPlugin::load()
  {
  _db.addType<SAppDatabase>();

  XProfiler::setStringForContext(ShiftCoreProfileScope, "ShiftCore");
  XProfiler::setStringForContext(ShiftDataModelProfileScope, "ShiftDataModel");

  xsize threadCount = 1;
  int idealThreadCount = QThread::idealThreadCount();
  if(idealThreadCount > 0)
    {
    threadCount = idealThreadCount;
    }

  SProcessManager::initiate(threadCount);
  }

void SPlugin::unload()
  {
  SProcessManager::terminate();
  }
