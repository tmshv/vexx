#include "taskdatabase.h"
#include "item.h"
#include "sdatabasemodel.h"
#include "styperegistry.h"
#include "QDir"
#include "QDesktopServices"
#include "sjsonio.h"

S_IMPLEMENT_PROPERTY(TaskDatabase)

SPropertyInformation *TaskDatabase::createTypeInformation()
  {
  return SPropertyInformation::create<TaskDatabase>("TaskDatabase");
  }

TaskDatabase::TaskDatabase() : SDatabase(), _rootItem(0)
  {
  initiateInheritedDatabaseType(staticTypeInformation());

  STypeRegistry::addType(Item::staticTypeInformation());
  STypeRegistry::addType(TaskDatabase::staticTypeInformation());
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
    SJSONSaver w;
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
    _rootItem = 0;
    SJSONLoader r;

    children.clear();
    r.readFromDevice(&file, this);

    SProperty *ent = children.findChild("Root Item");
    xAssert(ent);
    if(ent)
      {
      _rootItem = ent->castTo<Item>();
      }
    else
      {
      _rootItem = addItem(0, "Root Item");
      }
    xAssert(_rootItem);
    }
  }
