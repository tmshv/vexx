#include "scinterface.h"
#include "scplugin.h"

ScInterface::ScInterface( ScPlugin *in ) : _plugin( in )
    {
    }

void ScInterface::registerScriptObject( ScObject *in )
    {
    _plugin->registerScriptObject( in );
    }

void ScInterface::registerScriptGlobal( ScObject *in )
    {
    _plugin->registerScriptObject( in );
    }

void ScInterface::execute( QString in )
    {
    _plugin->execute( in );
    }
