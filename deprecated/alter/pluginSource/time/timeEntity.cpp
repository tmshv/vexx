#include "timeEntity.h"
#include "AEntityArray"
#include "ALog"

timeEntity::timeEntity( )
    {
    addProperty( "time/loop", FALSE, propertyInfo::Int );
    addProperty( "time/fps", 25.0, propertyInfo::Double );
    addProperty( "time/currentTime", 0.0, propertyInfo::Time );
    addProperty( "time/timeBegin", 0.0, propertyInfo::Time );
    addProperty( "time/timeEnd", 10, propertyInfo::Time );
    addProperty( "time/clipBegin", 0.0, propertyInfo::Time );
    addProperty( "time/clipEnd", 10.0, propertyInfo::Time );
    }
