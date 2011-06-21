#include "splugin.h"
#include "sglobal.h"
#include "sprocessmanager.h"
#include "QThread"
#include "QDesktopServices"
#include "QFile"
#include "QDir"
#include "sxmlio.h"
#include "styperegistry.h"

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
  STypeRegistry::initiate();
  STypeRegistry::addType(SAppDatabase::staticTypeInformation());

  XProfiler::setStringForContext(ShiftCoreProfileScope, "ShiftCore");
  XProfiler::setStringForContext(ShiftDataModelProfileScope, "ShiftDataModel");

  xsize threadCount = 1;
  int idealThreadCount = QThread::idealThreadCount();
  if(idealThreadCount > 0)
    {
    threadCount = idealThreadCount;
    }

  SProcessManager::initiate(threadCount);

  QString dataLocation = QDesktopServices::storageLocation(QDesktopServices::DataLocation);

  QFile file(dataLocation + "/settings.xml");
  if(file.open(QIODevice::ReadOnly))
    {
    SXMLLoader loader;

    loader.readFromDevice(&file, &_db.settings);
    }
  }

void SPlugin::unload()
  {
  QString dataLocation = QDesktopServices::storageLocation(QDesktopServices::DataLocation);

  QDir dir(QDir::root());
  dir.mkpath(dataLocation);

  QFile file(dataLocation + "/settings.xml");
  if(file.open(QIODevice::WriteOnly))
    {
    SXMLSaver saver;

    saver.writeToDevice(&file, &_db.settings);
    }

  SProcessManager::terminate();
  }
