#include "math/math.triple.h"
#include "math/math.vector2.h"

BEGIN_JLIB_MATH_NAMESPACE

triple::triple( ) : vectorX( 3 )
    {
    ;
    }

triple::triple( JFLOAT in ) : vectorX( 3, in )
    {
    ;
    }

triple::triple( QVector3D vec ) : vectorX( 3 )
    {
    x() = vec.x();
    y() = vec.y();
    z() = vec.z();
    }

triple::triple( vector2 xy, JFLOAT inZ ) : vectorX( 3 )
    {
    x() = xy.x();
    y() = xy.y();
    z() = inZ;
    }

triple::triple( JFLOAT *in ) : vectorX( 3, in )
    {
    ;
    }

triple::triple( JFLOAT X, JFLOAT Y, JFLOAT Z ) : vectorX( 3 )
    {
    x() = X;
    y() = Y;
    z() = Z;
    }

triple::triple( const triple &nv ) : vectorX( nv )
    {
    jAssert( nv._size == 3 );
    }

triple::triple( vectorX in ) : vectorX( in )
    {
    jAssert( _size > 2 );
    }

JFLOAT triple::dotProduct( triple in ) const
    {
    JFLOAT ret=0;
    for( unsigned int x=0; x<3; x++ )
        {
        ret += in.cmp[x] * cmp[x];
        }
    return ret;
    }

triple triple::crossProduct( triple b ) const
    {
    return triple( cmp[1]*b.cmp[2] - cmp[2]*b.cmp[1], cmp[2]*b.cmp[0] - cmp[0]*b.cmp[2], cmp[0]*b.cmp[1] - cmp[1]*b.cmp[0] );
    }

JFLOAT triple::dot( triple in ) const
    {
    JFLOAT ret=0;
    for( unsigned int x=0; x<3; x++ )
        {
        ret += in.cmp[x] * cmp[x];
        }
    return ret;
    }

triple triple::cross( triple b ) const
    {
    return triple( cmp[1]*b.cmp[2] - cmp[2]*b.cmp[1], cmp[2]*b.cmp[0] - cmp[0]*b.cmp[2], cmp[0]*b.cmp[1] - cmp[1]*b.cmp[0] );
    }

END_JLIB_MATH_NAMESPACE
