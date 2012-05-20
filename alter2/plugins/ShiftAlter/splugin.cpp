#include "splugin.h"
#include "sglobal.h"
#include "GraphicsCore/GCGlobal.h"
#include "GraphicsCore/3D/GCShader.h"
#include "sprocessmanager.h"
#include "spropertyinformationhelpers.h"
#include "QThread"
#include "QDesktopServices"
#include "QFile"
#include "QDir"
#include "Serialisation/sjsonio.h"
#include "styperegistry.h"
#include "saparteditorinterface.h"
#include "saparteditor.h"
#include "sashaderparteditorinterface.h"
#include "acore.h"
#include "mcglobal.h"
#include "meshcore.h"
#include "gcglobal.h"
#include "XArrayMath"
#include "spropertygroup.h"

namespace ShiftAlter
{
SPropertyGroup &propertyGroup()
  {
  static SPropertyGroup grp;
  return grp;
  }
}

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

  STypeRegistry::addPropertyGroup(ShiftAlter::propertyGroup());

  _mathsEngine = new XReferenceMathsEngine;
  XMathsEngine::setEngine(_mathsEngine);

  MathsCore::initiate();
  GraphicsCore::initiate();
  MeshCore::initiate();

  XProfiler::setStringForContext(GCProfileScope, "GraphicsCore");
  XProfiler::setStringForContext(496, "EksDataModel"); // X3DDataModelProfileScope
  XProfiler::setStringForContext(ShiftCoreProfileScope, "ShiftCore");
  XProfiler::setStringForContext(ShiftDataModelProfileScope, "ShiftDataModel");

  xsize threadCount = 1;
  int idealThreadCount = QThread::idealThreadCount();
  if(idealThreadCount > 0)
    {
    threadCount = idealThreadCount;
    }

  SProcessManager::initiate(threadCount);

  const_cast<SPropertyInformation*>(SProperty::staticTypeInformation())->
      addStaticInterface(new SDefaultPartEditorInterface);

  const_cast<SPropertyInformation*>(GCShader::staticTypeInformation())->
      addStaticInterface(new SShaderPartEditorInterface);

  _db = new SAppDatabase();
  _db->setPlugin(this);

  QDir dataDir = core()->localDataDirectory();
  QFile file(dataDir.absolutePath() + "/settings.json");
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
  STypeRegistry::terminate();

  XMathsEngine::setEngine(0);
  delete _mathsEngine;
  }
