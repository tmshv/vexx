#include "math/math.colour4.h"
#include "jLib.glHeader.h"

BEGIN_JLIB_MATH_NAMESPACE

colour4::colour4( ) : vectorX( 4, 1.0 )
    {
    ;
    }

colour4::colour4( float _l ) : vectorX( 4, _l ) {;}

colour4::colour4( QVector4D in ) : vectorX( 4 )
    {
    r() = in.x();
    g() = in.y();
    b() = in.z();
    a() = in.w();
    }

colour4::colour4( float _r, float _g, float _b, float _a ) : vectorX( 4 )
    {
    r() = _r;
    g() = _g;
    b() = _b;
    a() = _a;
    }
colour4::colour4( float _r, float _g, float _b ) : vectorX( 4 )
    {
    r() = _r;
    g() = _g;
    b() = _b;
    a() = 1;
    }

colour4 colour4::random( bool a )
    {
    math::colour4 temp;

    temp.r()=random::getF(0,1);
    temp.g()=random::getF(0,1);
    temp.b()=random::getF(0,1);
    if( a )
        {
        temp.a()=random::getF(0,1);
        }

    return temp;
    }

colour4::colour4( vectorX in ) : vectorX( 4 )
    {
    jAssert( in.size() >= 3 );
    r() = in[0];
    g() = in[1];
    b() = in[2];
    if( in.size() > 3 )
        {
        a() = in[3];
        }
    else
        {
        a() = 1;
        }
    }

colour4::colour4( const colour4 &nv ) : vectorX( nv )
    {
    jAssert( nv._size==4 );
    }

void colour4::load()
    {
#ifdef JFLOAT_IS_DOUBLE
    glColor4dv( cmp );
#else
    glColor4fv( cmp );
#endif
    }

const colour4 colour4::BLACK = colour4( 0, 0, 0 );
const colour4 colour4::WHITE = colour4( 1, 1, 1 );
const colour4 colour4::RED = colour4( 1, 0, 0 );
const colour4 colour4::GREEN = colour4( 0, 1, 0 );
const colour4 colour4::BLUE = colour4( 0, 0, 1 );
const colour4 colour4::YELLOW = colour4( 1, 1, 0 );
const colour4 colour4::MAGENTA = colour4( 1, 0, 1 );
const colour4 colour4::CYAN = colour4( 0, 1, 1 );

END_JLIB_MATH_NAMESPACE
