#include "splugin.h"

ALTER_PLUGIN(SPlugin);

SPlugin::SPlugin()
  {
  }

SDatabase &SPlugin::db()
  {
  return _db;
  }

const SDatabase &SPlugin::db() const
  {
  return _db;
  }

void SPlugin::load()
  {
  }

void SPlugin::unload()
  {
  }
