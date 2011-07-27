#ifndef TIMEENTITY_H
#define TIMEENTITY_H

#include "timeGlobal.h"
#include "AEntity"

class TIME_EXPORT timeEntity : public AEntity
    {
    A_ENTITY( timeEntity, "placid-acid.com", "Time" )
public:
    timeEntity();
    };


#endif // TIMEENTITY_H
