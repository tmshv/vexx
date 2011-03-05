#include "sccoreobject.h"
#include "scplugin.h"
#include "acore.h"
#include "QDebug"
#include "QDir"

ScCoreObject::ScCoreObject( ScPlugin *plugin ) : ScObject( "core" ), _plugin( plugin )
    {
    }

void ScCoreObject::triggerApplicationStarted()
    {
    emit applicationStarted();
    }

void ScCoreObject::triggerApplicationEnded()
    {
    emit applicationEnded();
    }

void ScCoreObject::quit()
    {
    _plugin->core()->quit();
    }

void ScCoreObject::include( QString name )
    {
    foreach( QString dirName, _plugin->core()->directories() )
        {
        QFile file( dirName + QDir::separator() + name );
        if( file.exists() && file.open( QIODevice::ReadOnly ) )
            {
            QString data( QString::fromUtf8( file.readAll() ) );
            _plugin->execute( data );
            }
        }
    }
