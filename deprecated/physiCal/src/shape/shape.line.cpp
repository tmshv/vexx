#include "shape/shape.line.h"
#include "shape/shape.plane.h"

BEGIN_PHYSICAL_NAMESPACE

line::line( pVector pos, pVector dir ) : position( pos ), direction( dir )
    {
    }

line::~line()
    {
    }

pReal line::shortestDistance( pVector j )
    {
    float length = direction.length();
    //jDebugNeat<<"Length = "<<length<<endl;
    if( length != 0 )
        {
        // This seems to work, but i knows not maths.
        pVector positionToJ = ( j - position );
        //pVector temp = ( ( j - position ) * direction );
        //jDebugNeat<<"Vector from line to point is "<<( j - position )<<endl;
        //return ( temp.x() + temp.y() + temp.z() ) / pow( length, 2 );
        pVector projectedDistance = direction.scale( positionToJ.dot( direction.normalise() ) );

        return ( projectedDistance - positionToJ ).length();
        }
    return floatLimits::infinity();
    }

pReal line::collision( pVector j )
    {
    float length = direction.length();
    //jDebugNeat<<"Length = "<<length<<endl;
    if( length != 0 )
        {
        pVector positionToJ = ( j - position );

        return positionToJ.dot( direction.normalise() ) / direction.length();
        }
    return floatLimits::infinity();
    }

pVector line::sample( pReal t )
    {
    return position + ( direction * t );
    }

pReal line::shortestDistance( line a )
    {
    pVector cP = direction.cross( a.direction );
    if( direction.normalise() == a.direction.normalise() )
        {
        return ( a.sample( plane( position, direction ).collision( a ) ) - position ).length();
        }
    if( cP != cP )
        {
        return 0;
        }
    return fabs( cP.dot( a.position - position ) / cP.length( ) );
    }

bool line::liesOn( pVector a )
    {
    pVector alpha = ( a - position ) / direction;
    if( alpha.x() == alpha.y() && alpha.x() == alpha.y() )
        {
        return TRUE;
        }
    return FALSE;
    }

pReal line::collision( line a )
    {
    if( a.direction != direction )
        {
        pVector cP = a.direction.cross( a.direction.cross( direction ) );
        return plane( a.position, cP ).collision( *this );
        }
    else if( liesOn( a.position ) )
        {
        return floatLimits::infinity();
        }
    else
        {
        return floatLimits::nan();
        }
    }


line lineToSpace( line in, math::matrix4x4 mat )
    {
    in.position = pVector( mat.pointMultiply( in.position ) );
    in.direction = pVector( mat.vectorMultiply( in.direction ) );

    return in;
    }

END_PHYSICAL_NAMESPACE
