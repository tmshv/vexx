#include "shape/shape.sphere.h"
#include "shape/shape.line.h"

BEGIN_PHYSICAL_NAMESPACE

sphere::sphere( pVector p, float r ) : position( p ), radius( r )
    {
    }

sphere::~sphere( )
    {
    }

void sphere::collision( line ln, pReal &i, pReal &j )
    {
    ln.position -= position;
    pReal a = ln.direction.dot( ln.direction );
    pReal b = 2 * ln.position.dot( ln.direction );
    pReal check = ( b * b ) - 4 * a * ( ln.position.dot( ln.position ) - ( radius * radius ) );

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

pReal sphere::shortestDistance( pVector a )
    {
    return ( ( position - a ).length() - radius );
    }

pReal sphere::shortestDistance( line a )
    {
    return a.shortestDistance( position ) - radius;
    }

END_PHYSICAL_NAMESPACE
