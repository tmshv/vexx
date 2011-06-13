#include "taskdatabase.h"
#include "item.h"
#include "sdatabasemodel.h"
#include "spropertydata.h"
#include "QDir"
#include "QDesktopServices"
#include "sxmlio.h"

S_ENTITY_EMPTY_DEFINITION(TaskDatabase, SDatabase)

TaskDatabase::TaskDatabase() : SDatabase(), _rootItem(0)
  {
  initiateInheritedDatabaseType(staticTypeInformation());

  addType<Item>();
  addType<TaskDatabase>();
  _rootItem = addItem(0, "Root Item");
  }

TaskDatabase::~TaskDatabase()
  {
  }

Item *TaskDatabase::addItem(Item *parentItem, QString name)
  {
  SEntity *parent = parentItem;
  if(!parent && _rootItem)
    {
    parent = _rootItem;
    }
  else if(!parent)
    {
    parent = this;
    }

  xAssert(parent);
  return parent->addChild<Item>(name);
  }

void TaskDatabase::removeItem(Item *it)
  {
  if(it)
    {
    SEntity* parent = it->parentEntity();
    if(parent && parent->castTo<Item>())
      {
      parent->children.remove(it);
      }
    }
  }

void TaskDatabase::save() const
  {
  QFile file(QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation) + QDir::separator() + "Tasks.db");
  if(file.open(QIODevice::WriteOnly))
    {
    SXMLSaver w;
    w.writeToDevice(&file, this);
    }
  else
    {
    qDebug() << "Couldn't open file for writing";
    }
  }

void TaskDatabase::load()
  {
  QFile file(QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation) + QDir::separator() + "Tasks.db");
  if(file.open(QIODevice::ReadOnly))
    {
    SXMLLoader r;
    r.readFromDevice(&file, this);
    }
  }
