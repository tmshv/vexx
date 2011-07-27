#include "viewportEntity.h"
#include "../SDK/AEntitySearch"
#include "../SDK/ALog"

viewportEntity::viewportEntity( ) : _object( 0 ), _group( 0 )
    {
    }

viewportEntity::~viewportEntity( )
    {
    }

void viewportEntity::setParent( AEntityLocation in )
    {
    AEntitySearch newParent = array()->search().findHasProperty( "viewport/object" ).findEntity( in );
    if( newParent.size() && static_cast <viewportEntity *>(&(newParent[0]))->group() )
        {
        AEntity::setParent( in );
        if( object() )
            {
            object()->setParent( static_cast <viewportEntity *>(&(newParent[0]))->group() );
            }
        }
    else if( in.isRoot() )
        {
        AEntity::setParent( in );
        object()->setParent( 0 );
        }
    }

void viewportEntity::object( jLib::graphics::entity::entityD *in )
    {
    _object = in;
    addProperty( "viewport/object", TRUE );
    property( "viewport/object" ).hide( TRUE );
    }

void viewportEntity::group( jLib::graphics::entity::entityG *in )
    {
    _group = in;
    addProperty( "viewport/group", FALSE );
    property( "viewport/group" ).hide( TRUE );
    }

void viewportEntity::prePropertyChanged( AProperty * )
    {
    update();
    }

void viewportEntity::relocated( )
    {
    update();
    }

void viewportEntity::visible( bool in )
    {
    if( _object )
        {
        _object->visible( in );
        }
    }
