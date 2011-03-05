#include "taskdatabase.h"
#include "item.h"
#include "sdatabasemodel.h"
#include "spropertydata.h"
#include "QDir"
#include "QDesktopServices"

S_ENTITY_EMPTY_DEFINITION(TaskDatabase, SDatabase)

TaskDatabase::TaskDatabase() : SDatabase(staticTypeInformation()), _rootItem(0)
  {
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
    SPropertyData data;
    write(0, data, SPropertyData::Binary);

    QDataStream stream(&file);
    stream << data;
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
    SPropertyData data;
    QDataStream stream(&file);
    stream >> data;
    children.clear();
    read(data, 0, SPropertyData::Binary);
    xAssert(children.firstChild());
    _rootItem = children.findChild("Root Item")->castTo<Item>();
    xAssert(_rootItem);
    }
  }
