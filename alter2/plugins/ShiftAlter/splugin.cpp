#include "splugin.h"
#include "sglobal.h"
#include "sprocessmanager.h"
#include "QThread"
#include "QDesktopServices"
#include "QFile"
#include "QDir"
#include "Serialisation/sjsonio.h"
#include "styperegistry.h"

ALTER_PLUGIN(SPlugin);

SPlugin::SPlugin() : _db(0)
  {
  }

SAppDatabase &SPlugin::db()
  {
  xAssert(_db);
  return *_db;
  }

const SAppDatabase &SPlugin::db() const
  {
  xAssert(_db);
  return *_db;
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

  _db = new SAppDatabase();

  QString dataLocation = QDesktopServices::storageLocation(QDesktopServices::DataLocation);

  QFile file(dataLocation + "/settings.json");
  if(file.open(QIODevice::ReadOnly))
    {
    SJSONLoader loader;

    loader.readFromDevice(&file, &_db->settings.children);
    }
  }

void SPlugin::unload()
  {
  QString dataLocation = QDesktopServices::storageLocation(QDesktopServices::DataLocation);

  QDir dir(QDir::root());
  dir.mkpath(dataLocation);

  QFile file(dataLocation + "/settings.json");
  if(file.open(QIODevice::WriteOnly))
    {
    SJSONSaver saver;
    saver.setAutoWhitespace(true);

    saver.writeToDevice(&file, &_db->settings.children, false);
    file.close();
    }

  delete _db;
  _db = 0;
  SProcessManager::terminate();
  }
