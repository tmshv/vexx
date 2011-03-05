#include "graphics/shape/graphics.shape.plane.h"


BEGIN_JLIB_GRAPHICS_SHAPE_NAMESPACE

plane::plane( math::triple p, math::triple n ) : position( p ), normal( n )
    {
    }

plane::~plane()
    {
    }

bool plane::liesOn( math::triple a )
    {
    return normal.dotProduct( a - position ) == 0;
    }

bool plane::liesOn( line a )
    {
    return liesOn( a.position ) && liesOn( a.position + a.direction );
    }

JFLOAT plane::collision( shape::line a )
    {
    //jDebugNeat<<"Normal: "<<normal<<", position: "<<position<<endl;
    //jDebugNeat<<"DIrection of line: "<<a.direction<<endl;
    JFLOAT denominator = normal.dotProduct( a.direction );
    //jDebugNeat<<"denominator is: "<<denominator<<endl;
    //jDebugNeat<<math::fcmp( denominator, 0 )<<endl;
    if( !math::fcmp( denominator, 0 ) )
        {
        return normal.dotProduct( position - a.position ) / denominator;
        }
    return floatLimits::infinity();
    }


bool plane::isInFront( const math::triple &point ) const
    {
    return normal.dot( point - position ) > 0;
    }
END_JLIB_GRAPHICS_SHAPE_NAMESPACE
