#include "BasicViewport.h"

ALTER_SDK_INITIALISE( VIEWPORT_EXPORT )
    {
    viewportDock *dock( new viewportDock( ) );
    sdk->addDockedItem( "Viewport", dock );
    sdk->addCore( dock );
    return APlugin::Success;
    }

ALTER_SDK_UNINITIALISE( VIEWPORT_EXPORT )
    {
    sdk->removeDockedItem( "Viewport" );
    }
