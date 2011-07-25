#include "math/math.matrix3x3.h"
#include "math/math.quaternion.h"
#include "math/math.triple.h"
#include "math/math.vector2.h"

BEGIN_JLIB_MATH_NAMESPACE

const matrix3x3 matrix3x3::IDENTITY = matrix3x3::identity();

matrix3x3::matrix3x3()
    {
    }

matrix3x3::matrix3x3( QMatrix3x3 in )
    {
    for( unsigned int x=0; x<3; x++ )
        {
        for( unsigned int y=0; y<3; y++ )
            {
            CM(x,y) = in(x,y);
            }
        }
    }

matrix3x3 matrix3x3::transpose() const
    {
    matrix3x3 ret;

    ret.RM(0,0) = CM(0,0);
    ret.RM(0,1) = CM(0,1);
    ret.RM(0,2) = CM(0,2);

    ret.RM(1,0) = CM(1,0);
    ret.RM(1,1) = CM(1,1);
    ret.RM(1,2) = CM(1,2);

    ret.RM(2,0) = CM(2,0);
    ret.RM(2,1) = CM(2,1);
    ret.RM(2,2) = CM(2,2);

    return ret;
    }

void matrix3x3::setScale( const triple &sc )
    {
    triple row1 = triple( RM(0,0), RM(1,0), RM(2,0) ).scale( sc.x() );
    triple row2 = triple( RM(0,1), RM(1,1), RM(2,1) ).scale( sc.y() );
    triple row3 = triple( RM(0,2), RM(1,2), RM(2,2) ).scale( sc.z() );

    RM(0,0) = row1.x();
    RM(1,0) = row1.y();
    RM(2,0) = row1.z();

    RM(0,1) = row2.x();
    RM(1,1) = row2.y();
    RM(2,1) = row2.z();

    RM(0,2) = row3.x();
    RM(1,2) = row3.y();
    RM(2,2) = row3.z();
    }

triple matrix3x3::scale( ) const
    {
    return triple( triple( RM(0,0), RM(1,0), RM(2,0) ).length(),
                   triple( RM(0,1), RM(1,1), RM(2,1) ).length(),
                   triple( RM(0,2), RM(1,2), RM(2,2) ).length() );
    }

quaternion matrix3x3::orientation( ) const
    {
    jAssert( scale() == 1 );

    quaternion ret;
    JFLOAT t = 1.0 + RM(0,0) + RM(1,1) + RM(2,2);

    if( t > 0.0001 )
        {
        JFLOAT s = 2 * sqrt( t );
        ret.w() = 0.25 * s;
        ret.x() = ( RM(1,2) - RM(2,1) ) / s;
        ret.y() = ( RM(2,0) - RM(0,2) ) / s;
        ret.z() = ( RM(0,1) - RM(1,0) ) / s;
        }
    else if( RM(0,0) > RM(1,1) && RM(0,0) > RM(2,2) )
        {
        jAssert( ( 1.0 + RM(0,0) - RM(1,1) - RM(2,2) ) > 0 );
        JFLOAT s = 2 * sqrt( 1.0 + RM(0,0) - RM(1,1) - RM(2,2) );
        ret.w() = ( RM(1,2) - RM(2,1) ) / s;
        ret.x() = 0.25 * s;
        ret.y() = ( RM(0,1) + RM(1,0) ) / s;
        ret.z() = ( RM(0,2) + RM(2,0) ) / s;
        }
    else if( RM(1,1) > RM(2,2) )
        {
        jAssert( ( 1.0 + RM(1,1) - RM(0,0) - RM(2,2) ) > 0 );
        JFLOAT s = 2 * sqrt( 1.0 + RM(1,1) - RM(0,0) - RM(2,2) );
        ret.w() = ( RM(2,0) - RM(0,2) ) / s;
        ret.x() = ( RM(0,1) + RM(1,0) ) / s;
        ret.y() = 0.25 * s;
        ret.z() = ( RM(1,2) + RM(2,1) ) / s;
        }
    else
        {
        jAssert( ( 1.0 + RM(2,2) - RM(1,1) - RM(0,0) ) > 0 );
        JFLOAT s = 2 * sqrt( 1.0 + RM(2,2) - RM(1,1) - RM(0,0) );
        ret.w() = ( RM(0,1) - RM(1,0) ) / s;
        ret.x() = ( RM(0,2) + RM(2,0) ) / s;
        ret.y() = ( RM(1,2) + RM(2,1) ) / s;
        ret.z() = 0.25 * s;
        }

    return ret;
    }

void matrix3x3::setOrientation( quaternion in )
    {
    jAssert( fcmp( in.length(), 1 ) );

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

    RM(0,0) = xx - yy - zz + ww;
    RM(1,0) = 2 * ( xy - zw );
    RM(2,0) = 2 * ( xz + yw );

    RM(0,1) = 2 * ( xy + zw );
    RM(1,1) = -xx + yy - zz + ww;
    RM(2,1) = 2 * ( yz - xw );

    RM(0,2) = 2 * ( xz - yw );
    RM(1,2) = 2 * ( yz + xw );
    RM(2,2) = -xx - yy + zz + ww;
#else
    RM(0,0) = 1.0 - 2.0 * ( yy + zz );
    RM(0,1) = 2 * ( xy + zw );
    RM(0,2) = 2 * ( xz - yw );

    RM(1,0) = 2 * ( xy - zw );
    RM(1,1) = 1 - 2 * ( xx + zz );
    RM(1,2) = 2 * ( yz + xw );

    RM(2,0) = 2 * ( xz + yw );
    RM(2,1) = 2 * ( yz - xw );
    RM(2,2) = 1 - 2 * ( xx + yy );
#endif
    }

matrix3x3 matrix3x3::inverse() const
    {
    jAssert( !fcmp( determinant(), 0 ) );
    JFLOAT invDet = 1/determinant();
    matrix3x3 r;

    r.RM(0,0) = ( RM(1,1) * RM(2,2) - RM(1,2) * RM(2,1) ) * invDet;
    r.RM(0,1) = -( RM(0,1) * RM(2,2) - RM(0,2) * RM(2,1) ) * invDet;
    r.RM(0,2) = ( RM(0,1) * RM(1,2) - RM(0,2) * RM(1,1) ) * invDet;

    r.RM(1,0) = -( RM(1,0) * RM(2,2) - RM(1,2) * RM(2,0) ) * invDet;
    r.RM(1,1) = ( RM(0,0) * RM(2,2) - RM(0,2) * RM(2,0) ) * invDet;
    r.RM(1,2) = -( RM(0,0) * RM(1,2) - RM(0,2) * RM(1,0) ) * invDet;

    r.RM(2,0) = ( RM(1,0) * RM(2,1) - RM(1,1) * RM(2,0) ) * invDet;
    r.RM(2,1) = -( RM(0,0) * RM(2,1) - RM(0,1) * RM(2,0) ) * invDet;
    r.RM(2,2) = ( RM(0,0) * RM(1,1) - RM(0,1) * RM(1,0) ) * invDet;

    return r;
    }

matrix3x3 matrix3x3::identity()
    {
    matrix3x3 ret;
    for( int x=0; x<3; x++ )
        {
        for( int y=0; y<3; y++ )
            {
            if( x==y )
                {
                ret.RM(x,y) = 1;
                }
            else
                {
                ret.RM(x,y) = 0;
                }
            }
        }
    return ret;
    }


matrix3x3 matrix3x3::multiply( const matrix3x3 &o ) const
    {
    matrix3x3 ret;
    for( int i = 0; i < 3; i++ )
        {
        for( int j = 0; j < 3; j++ )
            {
            ret.RM(i,j) = 0;
            for( int k = 0; k < 3; k++ )
                {
                ret.RM(i,j) += RM(i,k) * o.RM(k,j);
                }
            }
        }

    return ret;
    }

triple matrix3x3::multiply( const triple &o ) const
    {
    triple temp;
    for( int i = 0; i < 3; i++ )
        {
        temp[i]=0;
        for( int j = 0; j < 3; j++ )
            {
            temp[i] += RM(j,i) * o[j];
            }
        }
    return temp;
    }

JFLOAT matrix3x3::determinant() const
    {
    return ( RM(0,0)*RM(1,1)*RM(2,2) + RM(0,1)*RM(1,2)*RM(2,0) + RM(0,2)*RM(1,0)*RM(2,1) ) - ( RM(0,2)*RM(1,1)*RM(2,0) + RM(0,1)*RM(1,0)*RM(2,2) + RM(0,0)*RM(1,2)*RM(2,1) );
    }

bool matrix3x3::isEqual( const matrix3x3 &in ) const
    {
    for( unsigned int x=0; x<9; x++ )
        {
        if( !fcmp( _data[x], in._data[x] ) )
            {
            return FALSE;
            }
        }
    return TRUE;
    }

void matrix3x3::setTranslate( const vector2 &in )
    {
    RM(0,2) = in.x();
    RM(1,2) = in.y();
    }

vector2 matrix3x3::translate()
    {
    return vector2( RM(0,2), RM(1,2) );
    }
std::ostream& operator<<( std::ostream& output, matrix3x3 o )
    {
    return output<<o.RM(0,0)<<" "<<o.RM(0,1)<<" "<<o.RM(0,2)<<endl
                 <<o.RM(1,0)<<" "<<o.RM(1,1)<<" "<<o.RM(1,2)<<endl
                 <<o.RM(2,0)<<" "<<o.RM(2,1)<<" "<<o.RM(2,2)<<endl;
    }

END_JLIB_MATH_NAMESPACE
