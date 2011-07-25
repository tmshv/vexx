#include "graphics/primitives/graphics.primitives.pointCloud.h"
#include "graphics/entity/graphics.entity.bBox.h"

BEGIN_JLIB_GRAPHICS_PRIMITIVES_NAMESPACE

pointCloud::pointCloud( entity::entityG *p, float s ) : pointArray( p, s ), _usedPoints( 0 )
    {
    }

void pointCloud::removePoint( unsigned int x )
    {
    _usedPoints--;
    pointArray::removePoint( x );
    }

void pointCloud::clearPoints( )
    {
    _usedPoints = 0;
    pointArray::clearPoints( );
    }

void pointCloud::clear( )
    {
    clearPoints( );
    pointArray::clear( );
    }

unsigned int pointCloud::addPoint( math::triple p, math::colour4 c )
    {
    if( _usedPoints >= sizePoints() )
        {
        setPointArraySize( _usedPoints + 1 );
        }
    setPoint( _usedPoints++, p, c );
    return _usedPoints - 1;
    }

END_JLIB_GRAPHICS_PRIMITIVES_NAMESPACE
