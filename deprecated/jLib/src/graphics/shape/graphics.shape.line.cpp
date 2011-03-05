#include "graphics/shape/graphics.shape.line.h"
#include "graphics/shape/graphics.shape.plane.h"

BEGIN_JLIB_GRAPHICS_SHAPE_NAMESPACE

line::line( math::triple pos, math::triple dir ) : position( pos ), direction( dir )
    {
    }

line::~line()
    {
    }

JFLOAT line::shortestDistance( math::triple j )
    {
    float length = direction.length();
    //jDebugNeat<<"Length = "<<length<<endl;
    if( length != 0 )
        {
        // This seems to work, but i knows not maths.
        math::triple positionToJ = ( j - position );
        //math::triple temp = ( ( j - position ) * direction );
        //jDebugNeat<<"Vector from line to point is "<<( j - position )<<endl;
        //return ( temp.x() + temp.y() + temp.z() ) / pow( length, 2 );
        math::triple projectedDistance = direction.scale( positionToJ.dotProduct( direction.normalise() ) );

        return ( projectedDistance - positionToJ ).length();
        }
    return floatLimits::infinity();
    }

JFLOAT line::collision( math::triple j )
    {
    float length = direction.length();
    //jDebugNeat<<"Length = "<<length<<endl;
    if( length != 0 )
        {
        math::triple positionToJ = ( j - position );

        return positionToJ.dotProduct( direction.normalise() ) / direction.length();
        }
    return floatLimits::infinity();
    }

math::triple line::sample( JFLOAT t )
    {
    return position + ( direction * t );
    }

JFLOAT line::shortestDistance( line a )
    {
    math::triple cP = direction.crossProduct( a.direction );
    if( direction.normalise() == a.direction.normalise() )
        {
        return ( a.sample( plane( position, direction ).collision( a ) ) - position ).length();
        }
    if( cP != cP )
        {
        return 0;
        }
    return fabs( cP.dotProduct( a.position - position ) / cP.length( ) );
    }

bool line::liesOn( math::triple a )
    {
    math::triple alpha = ( a - position ) / direction;
    if( alpha.x() == alpha.y() && alpha.x() == alpha.y() )
        {
        return TRUE;
        }
    return FALSE;
    }

JFLOAT line::collision( line a )
    {
    if( a.direction != direction )
        {
        math::triple cP = a.direction.crossProduct( a.direction.crossProduct( direction ) );
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


shape::line lineToSpace( shape::line in, math::matrix4x4 mat )
    {
    in.position = mat.pointMultiply( in.position );
    in.direction = mat.vectorMultiply( in.direction );

    return in;
    }

END_JLIB_GRAPHICS_SHAPE_NAMESPACE
