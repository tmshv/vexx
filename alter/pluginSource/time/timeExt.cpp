#include "timeExt.h"

ALTER_SDK_INITIALISE( TIME_EXPORT )
    {
    sdk->addCreator( timeEntity::identifier(), timeEntity::create, FALSE );
    timeDock *dock = new timeDock( );
    sdk->addDockedItem( "Timeline", dock );
    sdk->addToolBar( "Time", new timeToolBar( dock ) );
    sdk->addCore( dock );
    return APlugin::Success;
    }

ALTER_SDK_UNINITIALISE( TIME_EXPORT )
    {
    sdk->removeCreator( timeEntity::identifier() );
    sdk->removeDockedItem( "Timeline" );
    sdk->removeToolBar( "Time" );
    }
