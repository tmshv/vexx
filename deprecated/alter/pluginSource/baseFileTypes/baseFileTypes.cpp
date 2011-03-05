#include "QtGlobal"
#include "xmlFileType.h"
#include "binaryFileType.h"

ALTER_SDK_INITIALISE( Q_DECL_EXPORT )
    {
    sdk->addFileType( "Alter XML", new xmlFileType );
    sdk->addFileType( "Alter Binary", new binaryFileType );
    return APlugin::Success;
    }

ALTER_SDK_UNINITIALISE( Q_DECL_EXPORT )
    {
    sdk->removeFileType( "Alter XML" );
    sdk->removeFileType( "Alter Binary" );
    }
