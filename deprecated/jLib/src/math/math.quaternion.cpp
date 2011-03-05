#include "math/math.quaternion.h"

BEGIN_JLIB_MATH_NAMESPACE

quaternion::quaternion( ) : vectorX( 4 )
    {
    }

quaternion::quaternion( const JFLOAT &W, const triple &in ) : vectorX( 4 )
    {
    w() = W;
    x() = in.x();
    y() = in.y();
    z() = in.z();
    }

quaternion::quaternion( JFLOAT X, JFLOAT Y, JFLOAT Z, JFLOAT W ) : vectorX( 4 )
    {
    x() = X;
    y() = Y;
    z() = Z;
    w() = W;
    }

quaternion::quaternion( const quaternion &nv ) : vectorX( nv )
    {
    jAssert( nv._size == 4 );
    }

quaternion::quaternion( vectorX in ) : vectorX( in )
    {
    jAssert( _size == 4 );
    }

triple quaternion::vector() const
    {
    return triple( x(), y(), z() ).normalise();
    }

JFLOAT quaternion::angleRadians() const
    {
    jAssert( jLib::math::fcmp( length(), 1.0 ) );
    return 2 * acos( r() );
    }

JFLOAT quaternion::angleDegrees() const
    {
    jAssert( jLib::math::fcmp( length(), 1.0 ) );
    return RADTODEG * 2 * acos( r() );
    }

void quaternion::setVector( const triple &in )
    {
    x() = in.x();
    y() = in.y();
    z() = in.z();
    *this = normalise();
    }

void quaternion::setAngleRadians( const JFLOAT in )
    {
    r() = cos( in / 2 );
    *this = normalise();
    }

void quaternion::setAngleDegrees( const JFLOAT in )
    {
    r() = cos( ( in * DEGTORAD ) / 2 );
    *this = normalise();
    }

void quaternion::setEulerAngles( triple in )
    {
    double c1 = cos( in.z() / 2 );
    double s1 = sin( in.z() / 2 );
    double c2 = cos( in.y() / 2 );
    double s2 = sin( in.y() / 2 );
    double c3 = cos( in.x() / 2 );
    double s3 = sin( in.x() / 2 );
    double c1c2 = c1*c2;
    double s1s2 = s1*s2;

    w() = c1c2*c3 - s1s2*s3;
    x() = c1c2*s3 + s1s2*c3;
    y() = s1*c2*c3 + c1*s2*s3;
    z() = c1*s2*c3 - s1*c2*s3;
    }

void quaternion::setAxisAngle( triple in, JFLOAT angle )
    {
    in = in.normalise();
    JFLOAT a( sin( angle / 2 ) );
    x() = in.x() * a;
    y() = in.y() * a;
    z() = in.z() * a;
    w() = cos( angle / 2 );
    }

triple quaternion::eulerAngles()
    {
    jAssert( fcmp( length(), 1 ) );
    double check = x() * y() + z() * w();
    if ( check > 0.4999 ) // at north pole
        {
        jDebugNeat<<"POLES!"<<endl;
        return triple( 0, M_PI_2, 2 * atan2( x(), w() ) );
        }
    if ( check < -0.4999 ) // at south pole
        {
        jDebugNeat<<"Also POLES"<<endl;
        return triple( 0, -M_PI_2, -2 * atan2( x(), w() ) );
        }
    return triple( atan2( 2*x()*w() - 2*y()*z(), 1 - 2*x()*x() - 2*z()*z() ),
                   asin( 2*x()*y() + 2*z()*w() ),
                   atan2( 2*y()*w() - 2*x()*z(), 1 - 2*y()*y() - 2*z()*z() ) );
    }

quaternion quaternion::multiply( const quaternion &in ) const
    {
    return quaternion( r()*in.i() + i()*in.r() + j()*in.k() - k()*in.j(),
                        r()*in.j() + j()*in.r() + k()*in.i() - i()*in.k(),
                        r()*in.k() + k()*in.r() + i()*in.j() - j()*in.i(),
                        r()*in.r() - i()*in.i() - j()*in.j() - k()*in.k() );
    }

quaternion quaternion::identity()
    {
    return quaternion( 0, 0, 0, 1 );
    }

quaternion quaternion::fromAxisAngle( triple a, JFLOAT b )
    {
    quaternion ret;
    ret.setAxisAngle( a, b );
    return ret;
    }

quaternion quaternion::fromEulerAngles( triple in )
    {
    quaternion ret;
    ret.setEulerAngles( in );
    return ret;
    }

const quaternion quaternion::IDENTITY = quaternion::identity();


END_JLIB_MATH_NAMESPACE

