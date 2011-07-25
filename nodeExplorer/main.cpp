#include "nodeExplorer.h"

ALTER_SDK_INITIALISE
    {
    sdk->addDockedItem( "Node Explorer", new nodeExplorerDock );

    return alterSDKBase::Success;
    }

ALTER_SDK_UNINITIALISE
    {
    sdk->removeDockedItem( "Node Explorer" );
    }
