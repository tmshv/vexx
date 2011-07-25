#include "math/math.polar.h"

BEGIN_JLIB_MATH_NAMESPACE

polar::polar()
    {
    radius() = 0;
    phi() = 0;
    theta() = 0;
    }

polar::polar( const triple &in )
    {
    radius() = in.length();
    if( radius() != 0 )
        {
        phi() = atan2( in.z(), in.x() );
        theta() = acos( in.y() / radius() );
        }
    else
        {
        phi() = 0;
        theta() = 0;
        }
    }

triple polar::getPoint()
    {
    triple ret;
    ret.x() = radius() * cos( phi() ) * sin( theta() );
    ret.z() = radius() * sin( phi() ) * sin( theta() );
    ret.y() = radius() * cos( theta() );
    return ret;
    }

END_JLIB_MATH_NAMESPACE
