#include <QtGui/QApplication>
#include "APlugin"
#include "debugDock.h"

ALTER_SDK_INITIALISE( Q_DECL_EXPORT )
    {
    sdk->addDockedItem( "Debug Tree", new debugDock );
    return APlugin::Success;
    }

ALTER_SDK_UNINITIALISE( Q_DECL_EXPORT )
    {
    sdk->removeDockedItem( "Debug Tree" );
    }
