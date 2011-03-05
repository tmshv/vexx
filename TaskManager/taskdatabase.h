#ifndef TASKDATABASE_H
#define TASKDATABASE_H

#include "XProperty"
#include "sdatabase.h"
#include "taskglobal.h"

class Item;
class SDatabaseModel;

class TaskDatabase : public SDatabase
  {
  S_ENTITY(TaskDatabase, SDatabase);
  XROProperty(Item *, rootItem);

public:
  TaskDatabase();
  ~TaskDatabase();

  Item *addItem(Item *parent=0, QString name="New Item");
  void removeItem(Item *);

  void save() const;
  void load();
  };

#endif // TASKDATABASE_H
