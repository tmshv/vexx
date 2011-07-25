#include "graphics/shape/graphics.shape.sphere.h"
#include "graphics/shape/graphics.shape.line.h"

BEGIN_JLIB_GRAPHICS_SHAPE_NAMESPACE

sphere::sphere( math::triple p, float r ) : position( p ), radius( r )
    {
    }

sphere::~sphere( )
    {
    }

void sphere::collision( line ln, JFLOAT &i, JFLOAT &j )
    {
    ln.position -= position;
    JFLOAT a = ln.direction.dotProduct( ln.direction );
    JFLOAT b = 2 * ln.position.dotProduct( ln.direction );
    JFLOAT check = ( b * b ) - 4 * a * ( ln.position.dotProduct( ln.position ) - ( radius * radius ) );

    if( check < 0 )
        {
        i = j = floatLimits::nan();
        return;
        }

    a *= 2;
    check = sqrt( check );

    i = ( -b + check ) / a;
    j = ( -b - check ) / a;
    }

bool sphere::collision( line a )
    {
    return shortestDistance( a ) < radius;
    }

JFLOAT sphere::shortestDistance( math::triple a )
    {
    return ( ( position - a ).length() - radius );
    }

JFLOAT sphere::shortestDistance( line a )
    {
    return a.shortestDistance( position ) - radius;
    }

END_JLIB_GRAPHICS_SHAPE_NAMESPACE
