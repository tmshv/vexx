#include "script.h"

ALTER_SDK_INITIALISE
    {
    sdk->addDockedItem( "Script Editor", new scriptDock );
    return alterSDKBase::Success;
    }

ALTER_SDK_UNINITIALISE
    {
    sdk->removeDockedItem( "Script Editor" );
    }
