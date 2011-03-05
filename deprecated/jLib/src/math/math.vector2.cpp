#include "math/math.vector2.h"

BEGIN_JLIB_MATH_NAMESPACE

vector2::vector2( ) : vectorX( 2, 0.0 )
    {;}

vector2::vector2( float in ) : vectorX( 2, in )
    {;}

vector2::vector2( float X, float Y ) : vectorX( 2 )
    {
    x() = X;
    y() = Y;
    }

vector2::vector2( const vector2 &nv ) : vectorX( nv )
    {jAssert( nv._size==2 );}

vector2::vector2( vectorX in ) : vectorX( 2 )
    {
    jAssert( in.size() >= 2 );
    x() = in[0];
    y() = in[1];
    }

END_JLIB_MATH_NAMESPACE
