#include "shape/shape.plane.h"

BEGIN_PHYSICAL_NAMESPACE

plane::plane( pVector p, pVector n ) : position( p ), normal( n )
    {
    }

plane::~plane()
    {
    }

bool plane::liesOn( pVector a )
    {
    return normal.dot( a - position ) == 0;
    }

bool plane::liesOn( line a )
    {
    return liesOn( a.position ) && liesOn( a.position + a.direction );
    }

pReal plane::collision( line a ) const
    {
    //jDebugNeat<<"Normal: "<<normal<<", position: "<<position<<endl;
    //jDebugNeat<<"DIrection of line: "<<a.direction<<endl;
    pReal denominator = normal.dot( a.direction );
    //jDebugNeat<<"denominator is: "<<denominator<<endl;
    //jDebugNeat<<math::fcmp( denominator, 0 )<<endl;
    if( !math::fcmp( denominator, 0 ) )
        {
        return normal.dot( position - a.position ) / denominator;
        }
    return floatLimits::infinity();
    }

bool plane::isInFront( const pVector &point ) const
    {
    return normal.dot( point - position ) > 0;
    }

END_PHYSICAL_NAMESPACE
