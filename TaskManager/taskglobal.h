#ifndef TASKGLOBAL_H
#define TASKGLOBAL_H

#include "sglobal.h"

#define TASK_TYPE(name, id) S_PROPERTY_TYPE(name, 1000, id)

TASK_TYPE(TaskDatabase, 1);
TASK_TYPE(Item, 2);

#endif // TASKGLOBAL_H
