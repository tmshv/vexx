#include "splugin.h"
#include "sglobal.h"

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
  }

void SPlugin::unload()
  {
  }
