#include "math/math.matrix4x3.h"
#include "math/math.triple.h"
#include "math/math.matrix4x4.h"
#include "math/math.matrix3x3.h"

BEGIN_JLIB_MATH_NAMESPACE

const matrix4x3 matrix4x3::IDENTITY = matrix4x3::identity();

std::ostream& JLIB_EXPORT operator<<(std::ostream& in, const matrix4x3 &mat )
    {
    for( unsigned int i=0; i<4; i++ )
        {
        for( unsigned int j=0; j<4; j++ )
            {
            in<<mat.RM(i,j)<<", ";
            }
        in<<endl;
        }
    return in;
    }

matrix4x3::matrix4x3()
    {
    setHomogeneous();
    }

matrix4x3::matrix4x3( triple t, quaternion o, triple s )
    {
    setTranslation( t );
    setOrientation( o );
    setScale( s );
    setHomogeneous();
    }

void matrix4x3::setOrientation( quaternion in )
    {
    jAssert( fcmp( in.length(), 1 ) );
    triple sc = scale();
    if( sc == 0 )
        {
        sc = 1;
        }

    JFLOAT xx = in.x() * in.x();
    JFLOAT xy = in.x() * in.y();
    JFLOAT xz = in.x() * in.z();
    JFLOAT xw = in.x() * in.w();

    JFLOAT yy = in.y() * in.y();
    JFLOAT yz = in.y() * in.z();
    JFLOAT yw = in.y() * in.w();

    JFLOAT zz = in.z() * in.z();
    JFLOAT zw = in.z() * in.w();


#ifndef USEALTERNATIVEQUATTOMATRIX
    JFLOAT ww = in.w() * in.w();

    CM(0,0) = xx - yy - zz + ww;
    CM(0,1) = 2 * ( xy - zw );
    CM(0,2) = 2 * ( xz + yw );

    CM(1,0) = 2 * ( xy + zw );
    CM(1,1) = -xx + yy - zz + ww;
    CM(1,2) = 2 * ( yz - xw );

    CM(2,0) = 2 * ( xz - yw );
    CM(2,1) = 2 * ( yz + xw );
    CM(2,2) = -xx - yy + zz + ww;
#else
    CM(0,0) = 1.0 - 2.0 * ( yy + zz );
    CM(1,0) = 2 * ( xy + zw );
    CM(2,0) = 2 * ( xz - yw );

    CM(0,1) = 2 * ( xy - zw );
    CM(1,1) = 1 - 2 * ( xx + zz );
    CM(2,1) = 2 * ( yz + xw );

    CM(0,2) = 2 * ( xz + yw );
    CM(1,2) = 2 * ( yz - xw );
    CM(2,2) = 1 - 2 * ( xx + yy );
#endif
    setScale( sc );
    }

void matrix4x3::setScale( triple in )
    {
    triple s = scale();

    RM(0,0) *= in.x() / s.x();
    RM(0,1) *= in.x() / s.x();
    RM(0,2) *= in.x() / s.x();

    RM(1,0) *= in.y() / s.y();
    RM(1,1) *= in.y() / s.y();
    RM(1,2) *= in.y() / s.y();

    RM(2,0) *= in.z() / s.z();
    RM(2,1) *= in.z() / s.z();
    RM(2,2) *= in.z() / s.z();
    }

void matrix4x3::setTranslation( triple t )
    {
    RM(0,3) = t.x();
    RM(1,3) = t.y();
    RM(2,3) = t.z();
    }

triple matrix4x3::scale() const
    {
    return triple( triple( RM(0,0), RM(0,1), RM(0,2) ).length(),
                   triple( RM(1,0), RM(1,1), RM(1,2) ).length(),
                   triple( RM(2,0), RM(2,1), RM(2,2) ).length() );
    }

triple matrix4x3::translation() const
    {
    return triple( RM(0,3), RM(1,3), RM(2,3) );
    }

quaternion matrix4x3::orientation() const
    {
    jAssert( scale() == 1 );
    quaternion ret;
    JFLOAT t = 1.0 + CM(0,0) + CM(1,1) + CM(2,2);

    if( t > 0.0001 )
        {
        JFLOAT s = 2 * sqrt( t );
        ret.w() = 0.25 * s;
        ret.x() = ( CM(2,1) - CM(1,2) ) / s;
        ret.y() = ( CM(0,2) - CM(2,0) ) / s;
        ret.z() = ( CM(1,0) - CM(0,1) ) / s;
        }
    else if( CM(0,0) > CM(1,1) && CM(0,0) > CM(2,2) )
        {
        jAssert( ( 1.0 + CM(0,0) - CM(1,1) - CM(2,2) ) > 0 );
        JFLOAT s = 2 * sqrt( 1.0 + CM(0,0) - CM(1,1) - CM(2,2) );
        ret.w() = ( CM(2,1) - CM(1,2) ) / s;
        ret.x() = 0.25 * s;
        ret.y() = ( CM(1,0) + CM(0,1) ) / s;
        ret.z() = ( CM(2,0) + CM(0,2) ) / s;
        }
    else if( CM(1,1) > CM(2,2) )
        {
        jAssert( ( 1.0 + CM(1,1) - CM(0,0) - CM(2,2) ) > 0 );
        JFLOAT s = 2 * sqrt( 1.0 + CM(1,1) - CM(0,0) - CM(2,2) );
        ret.w() = ( CM(0,2) - CM(2,0) ) / s;
        ret.x() = ( CM(1,0) + CM(0,1) ) / s;
        ret.y() = 0.25 * s;
        ret.z() = ( CM(2,1) + CM(1,2) ) / s;
        }
    else
        {
        jAssert( ( 1.0 + CM(2,2) - CM(1,1) - CM(0,0) ) > 0 );
        JFLOAT s = 2 * sqrt( 1.0 + CM(2,2) - CM(1,1) - CM(0,0) );
        ret.w() = ( CM(1,0) - CM(0,1) ) / s;
        ret.x() = ( CM(2,0) + CM(0,2) ) / s;
        ret.y() = ( CM(2,1) + CM(1,2) ) / s;
        ret.z() = 0.25 * s;
        }
    return ret;
    }

matrix4x3 matrix4x3::inverse() const
    {
    jAssert( scale() == 1 );
    matrix4x3 ret = transpose();
    ret.setHomogeneous();
    ret.setTranslation( translation().negate() );
    return ret;
    }

void matrix4x3::setToInverse()
    {
    *this = inverse();
    }

matrix4x3 matrix4x3::transpose() const
    {
    matrix4x3 ret;

    for( unsigned int i=0; i<4; ++i )
        {
        for( unsigned int j=0; j<4; ++j )
            {
            ret._RM( i, j ) = CM( i, j );
            }
        }

    return ret;
    }

void matrix4x3::setToTranspose()
    {
    *this = transpose();
    }

triple matrix4x3::pointMultiply( const triple &vec ) const
    {
    return triple( vec.x() * RM(0,0) + vec.y() * RM(1,0) + vec.z() * RM(2,0) + RM(0,3),
                   vec.x() * RM(0,1) + vec.y() * RM(1,1) + vec.z() * RM(2,1) + RM(1,3),
                   vec.x() * RM(0,2) + vec.y() * RM(1,2) + vec.z() * RM(2,2) + RM(2,3) );
    }

triple matrix4x3::vectorMultiply( const triple &vec ) const
    {
    return triple( vec.x() * RM(0,0) + vec.y() * RM(1,0) + vec.z() * RM(2,0),
                   vec.x() * RM(0,1) + vec.y() * RM(1,1) + vec.z() * RM(2,1),
                   vec.x() * RM(0,2) + vec.y() * RM(1,2) + vec.z() * RM(2,2) );
    }

matrix4x3 matrix4x3::multiply( const matrix4x3 &o ) const
    {
    matrix4x3 r;
    r.RM(0,0) = (o.RM(0,0)*RM(0,0)) + (o.RM(0,1)*RM(1,0)) + (o.RM(0,2)*RM(2,0));
    r.RM(0,1) = (o.RM(0,0)*RM(0,1)) + (o.RM(0,1)*RM(1,1)) + (o.RM(0,2)*RM(2,1));
    r.RM(0,2) = (o.RM(0,0)*RM(0,2)) + (o.RM(0,1)*RM(1,2)) + (o.RM(0,2)*RM(2,2));

    r.RM(1,0) = (o.RM(1,0)*RM(0,0)) + (o.RM(1,1)*RM(1,0)) + (o.RM(1,2)*RM(2,0));
    r.RM(1,1) = (o.RM(1,0)*RM(0,1)) + (o.RM(1,1)*RM(1,1)) + (o.RM(1,2)*RM(2,1));
    r.RM(1,2) = (o.RM(1,0)*RM(0,2)) + (o.RM(1,1)*RM(1,2)) + (o.RM(1,2)*RM(2,2));

    r.RM(2,0) = (o.RM(2,0)*RM(0,0)) + (o.RM(2,1)*RM(1,0)) + (o.RM(2,2)*RM(2,0));
    r.RM(2,1) = (o.RM(2,0)*RM(0,1)) + (o.RM(2,1)*RM(1,1)) + (o.RM(2,2)*RM(2,1));
    r.RM(2,2) = (o.RM(2,0)*RM(0,2)) + (o.RM(2,1)*RM(1,2)) + (o.RM(2,2)*RM(2,2));

    r.RM(0,3) = (o.RM(0,3)*RM(0,0)) + (o.RM(1,3)*RM(0,1)) + (o.RM(2,3)*RM(0,2)) + RM(0,3);
    r.RM(1,3) = (o.RM(0,3)*RM(1,0)) + (o.RM(1,3)*RM(1,1)) + (o.RM(2,3)*RM(1,2)) + RM(1,3);
    r.RM(2,3) = (o.RM(0,3)*RM(2,0)) + (o.RM(1,3)*RM(2,1)) + (o.RM(2,3)*RM(2,2)) + RM(2,3);

    return r;
    }

matrix3x3 matrix4x3::multiply( const matrix3x3 &o ) const
    {
    matrix3x3 r;
    r.RM(0,0) = (o.RM(0,0)*RM(0,0)) + (o.RM(0,1)*RM(1,0)) + (o.RM(0,2)*RM(2,0));
    r.RM(0,1) = (o.RM(0,0)*RM(0,1)) + (o.RM(0,1)*RM(1,1)) + (o.RM(0,2)*RM(2,1));
    r.RM(0,2) = (o.RM(0,0)*RM(0,2)) + (o.RM(0,1)*RM(1,2)) + (o.RM(0,2)*RM(2,2));

    r.RM(1,0) = (o.RM(1,0)*RM(0,0)) + (o.RM(1,1)*RM(1,0)) + (o.RM(1,2)*RM(2,0));
    r.RM(1,1) = (o.RM(1,0)*RM(0,1)) + (o.RM(1,1)*RM(1,1)) + (o.RM(1,2)*RM(2,1));
    r.RM(1,2) = (o.RM(1,0)*RM(0,2)) + (o.RM(1,1)*RM(1,2)) + (o.RM(1,2)*RM(2,2));

    r.RM(2,0) = (o.RM(2,0)*RM(0,0)) + (o.RM(2,1)*RM(1,0)) + (o.RM(2,2)*RM(2,0));
    r.RM(2,1) = (o.RM(2,0)*RM(0,1)) + (o.RM(2,1)*RM(1,1)) + (o.RM(2,2)*RM(2,1));
    r.RM(2,2) = (o.RM(2,0)*RM(0,2)) + (o.RM(2,1)*RM(1,2)) + (o.RM(2,2)*RM(2,2));

    return r;
    }

matrix4x4 matrix4x3::asMatrix4x4() const
    {
    matrix4x4 ret;

    for( unsigned int i=0; i<4; ++i )
        {
        for( unsigned int j=0; j<4; ++j )
            {
            ret.RM( i, j ) = RM( i, j );
            }
        }

    return ret;
    }

matrix3x3 matrix4x3::asMatrix3x3() const
    {
    matrix3x3 ret = matrix3x3::IDENTITY;

    for( unsigned int i=0; i<3; ++i )
        {
        for( unsigned int j=0; j<3; ++j )
            {
            ret.RM( i, j ) = RM( i, j );
            }
        }

    return ret;
    }

bool matrix4x3::isEqual( const matrix4x3 &in )
    {
    for( unsigned int x=0; x<12; x++ )
        {
        if( !fcmp( _data[x], in._data[x] ) )
            {
            return FALSE;
            }
        }
    return TRUE;
    }

matrix4x3 matrix4x3::identity()
    {
    matrix4x3 ret;

    for( unsigned int i=0; i<4; i++ )
        {
        for( unsigned int j=0; j<4; j++ )
            {
            if( i == j )
                {
                ret._RM(i,j) = 1;
                }
            else
                {
                ret._RM(i,j) = 0;
                }
            }
        }

    return ret;
    }

END_JLIB_MATH_NAMESPACE
