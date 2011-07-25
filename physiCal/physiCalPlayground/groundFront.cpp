#include "groundFront.h"

propertyEntity *groundFront::create( )
    {
    return new groundFront( );
    }

groundFront::groundFront( ) : viewportEntity( ), _grid(0)
    {
    object( &_grid );
    addProperty( "lattice/material", "", "Material" );
    bounds().minimum( jLib::math::triple( -5, -0.1, -5 ) );
    bounds().maximum( jLib::math::triple( 5, 0.1, 5 ) );
    }

void groundFront::propertyChanged( QString name )
    {
    viewportEntity::propertyChanged( name );
    }
