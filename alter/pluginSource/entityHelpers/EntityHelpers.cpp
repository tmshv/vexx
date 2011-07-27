#include "APlugin"

#include "EntityHelpers.h"

#include "propertiesDock.h"
#include "explorerDock.h"

ALTER_SDK_INITIALISE( Q_DECL_EXPORT )
    {
    sdk->addDockedItem( "Explorer", new explorerDock( ) );
    sdk->addDockedItem( "Properties", new propertiesDock( ) );
    return APlugin::Success;
    }

ALTER_SDK_UNINITIALISE( Q_DECL_EXPORT )
    {
    sdk->removeDockedItem( "Explorer" );
    sdk->removeDockedItem( "Properties" );
    }
