#include "math/math.matrix4x4.h"
#include "math/math.quaternion.h"
#include "math/math.matrix3x3.h"
#include "math/math.matrix4x3.h"
#include "math/math.triple.h"
#include "jLib.glHeader.h"

BEGIN_JLIB_MATH_NAMESPACE

/*matrix4x4::matrix4x4( JFLOAT _a, JFLOAT _b, JFLOAT _c, JFLOAT _d,
        JFLOAT _e, JFLOAT _f, JFLOAT _g, JFLOAT _h,
        JFLOAT _i, JFLOAT _j, JFLOAT _k, JFLOAT _l,
        JFLOAT _m, JFLOAT _n, JFLOAT _o, JFLOAT _p )
    {
    _data[0] = _a;
    _data[1] = _b;
    _data[2] = _c;
    _data[3] = _d;

    _data[4] = _e;
    _data[5] = _f;
    _data[6] = _g;
    _data[7] = _h;

    _data[8] = _i;
    _data[9] = _j;
    _data[10] = _k;
    _data[11] = _l;

    _data[12] = _m;
    _data[13] = _n;
    _data[14] = _o;
    _data[15] = _p;
    }*/
/*matrix4x4::matrix4x4( JFLOAT *in )
    {
    for( int x=0; x<16; x++ )
        {
        cmp[x] = in[x];
        }
    }*/

/*matrix4x4 matrix4x4::translate( triple t )
    {
    matrix4x4 ret;

    ret.cmp[12] = t.x();
    ret.cmp[13] = t.y();
    ret.cmp[14] = t.z();

    return ret;
    }

matrix4x4 matrix4x4::rotate( enum axis a, float angle )
    {
    matrix4x4 ret;
    float c = cos(DEGTORAD*angle), s = sin(DEGTORAD*angle);
    if( a == Z )
        {
        ret.RM(0,0) = c;
        ret.RM(1,0) = s;
        ret.RM(0,1) = -s;
        ret.RM(1,1) = c;
        }
    else if( a == Y )
        {
        ret.RM(0,0) = c;
        ret.RM(2,0) = -s;
        ret.RM(0,2) = s;
        ret.RM(2,2) = c;
        }
    else if( a == X )
        {
        ret.RM(1,1) = c;
        ret.RM(2,1) = s;
        ret.RM(1,2) = -s;
        ret.RM(2,2) = c;
        }
    return ret;
    }

matrix4x4 matrix4x4::scale( triple s )
    {
    matrix4x4 ret;

    ret.cmp[0] = s.x();
    ret.cmp[5] = s.y();
    ret.cmp[10] = s.z();

    return ret;
    }*/

/*JFLOAT *matrix4x4::ptr()
    {
    return cmp;
    }

const JFLOAT *matrix4x4::ptr() const
    {
    return cmp;
    }*/

void matrix4x4::setScale( const triple &sc )
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

triple matrix4x4::scale( ) const
    {
    return triple( triple( RM(0,0), RM(1,0), RM(2,0) ).length(),
                   triple( RM(0,1), RM(1,1), RM(2,1) ).length(),
                   triple( RM(0,2), RM(1,2), RM(2,2) ).length() );
    }

matrix4x4 matrix4x4::normalise( ) const
    {
    matrix4x4 ret( *this );
    ret.setScale( 1 );
    return ret;
    }

triple matrix4x4::xAxis( ) const
    {
    return triple( RM(0,0), RM(1,0), RM(2,0) );
    }

triple matrix4x4::yAxis( ) const
    {
    return triple( RM(0,1), RM(1,1), RM(2,1) );
    }

triple matrix4x4::zAxis( ) const
    {
    return triple( RM(0,2), RM(1,2), RM(2,2) );
    }

void matrix4x4::setXAxis( triple in )
    {
    RM(0,0) = in.x();
    RM(1,0) = in.y();
    RM(2,0) = in.z();
    }

void matrix4x4::setYAxis( triple in )
    {
    RM(0,1) = in.x();
    RM(1,1) = in.y();
    RM(2,1) = in.z();
    }

void matrix4x4::setZAxis( triple in )
    {
    RM(0,2) = in.x();
    RM(1,2) = in.y();
    RM(2,2) = in.z();
    }

/*void matrix4x4::zero()
    {
    for( int x=0; x<16; x++ )
        {
        cmp[x] = 0;
        }
    }*/

/*matrix4x4 matrix4x4::fillModelView()
    {
    matrix4x4 temp;
    glGetFloatv( GL_MODELVIEW_MATRIX, temp.cmp );
    return temp;
    }

matrix4x4 matrix4x4::fillProjection()
    {
    matrix4x4 temp;
    glGetFloatv( GL_PROJECTION_MATRIX, cmp );
    return temp;
    }*/

void matrix4x4::multiplyCurrent()
    {
#ifdef JFLOAT_IS_DOUBLE
    glMultMatrixd( _data );
#else
    glMultMatrixf( _data );
#endif
    }

matrix4x4 matrix4x4::transpose() const
    {
    matrix4x4 ret;
    for( unsigned int x=0; x<4; x++ )
        {
        for( unsigned int y=0; y<4; y++ )
            {
            ret.RM(x,y) = CM(x,y);
            }
        }
    return ret;
    }

bool matrix4x4::isNormalised() const
    {
    return scale() == 1;
    }

quaternion matrix4x4::orientation( ) const
    {
    jAssert( (*this).isNormalised() );
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

void matrix4x4::setOrientation( quaternion in )
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

triple matrix4x4::translation() const
    {
    return triple( RM(0,3), RM(1,3), RM(2,3) );
    }

void matrix4x4::setTranslation( triple in )
    {
    RM(0,3) = in.x();
    RM(1,3) = in.y();
    RM(2,3) = in.z();
    }

matrix3x3 matrix4x4::minor3x3( unsigned int _i, unsigned int _j ) const
    {
    matrix3x3 ret;

    for( unsigned int i=0; i<_i; i++ )
        {
        for( unsigned int j=0; j<_j; j++ )
            {
            ret.RM(j,i) = RM(j,i);
            }
        }

    for( unsigned int i=_i+1; i<4; i++ )
        {
        for( unsigned int j=0; j<_j; j++ )
            {
            ret.RM(j,i-1) = RM(j,i);
            }
        }

    for( unsigned int i=0; i<_i; i++ )
        {
        for( unsigned int j=_j+1; j<4; j++ )
            {
            ret.RM(j-1,i) = RM(j,i);
            }
        }

    for( unsigned int i=_i+1; i<4; i++ )
        {
        for( unsigned int j=_j+1; j<4; j++ )
            {
            ret.RM(j-1,i-1) = RM(j,i);
            }
        }

    return ret;
    }

matrix4x4 matrix4x4::cofactor() const
    {
    matrix4x4 ret;

    for( int x=0; x<4; x++ )
        {
        for( int y=0; y<4; y++ )
            {
            ret.RM(y,x) = minor3x3(x,y).determinant() * pow((float)-1,x+y);
            }
        }

    return ret;
    }

JFLOAT matrix4x4::determinant() const
    {
    JFLOAT ret=0;

    for( int x=0; x<4; x++ )
        {
        ret += pow( -1.0f, x ) * minor3x3( 0, x ).determinant() * RM(x,0);
        }

    return ret;
    }

matrix4x4 matrix4x4::inverse() const
    {
    jAssert( !fcmp( determinant(), 0 ) );
    return ( cofactor().transpose() / determinant() );
    }

bool matrix4x4::isEqual( const matrix4x4 &other ) const
    {
    for( int x=0; x<16; x++ )
        {
        if( !fcmp(_data[x],other._data[x]) )
            {
            return FALSE;
            }
        }
    return TRUE;
    }

matrix4x4 matrix4x4::multiply( const matrix4x4 &other ) const
    {
    matrix4x4 ret;
    for( int i = 0; i < 4; i++ )
        {
        for( int j = 0; j < 4; j++ )
            {
            ret.RM(j,i) = 0;
            for( int k = 0; k < 4; k++ )
                {
                ret.RM(j,i) += RM(k,i) * other.RM(j,k);
                }
            }
        }

    return ret;
    }

triple matrix4x4::pointMultiply( triple in )
    {
    triple ret;
    for( int i=0; i<3; i++ )
        {
        ret[i]=0;
        for( int j = 0; j < 3; j++ )
            {
            ret[i] += RM(i,j) * in[j];
            }
        }

    ret.x() += RM(0,3);
    ret.y() += RM(1,3);
    ret.z() += RM(2,3);

    return ret;
    }

triple matrix4x4::vectorMultiply( triple in )
    {
    triple ret;
    for( int i=0; i<3; i++ )
        {
        ret[i]=0;
        for( int j = 0; j < 3; j++ )
            {
            ret[i] += RM(i,j) * in[j];
            }
        }

    return ret;
    }

/*quickPt3 matrix4x4::operator *( quickPt3 other )
    {

    quickPt3 temp;
    for( int i = 0; i < 3; i++ )
        {
        temp.cmp[i]=0;
        for( int j = 0; j < 3; j++ )
            {
            temp.cmp[i] += cmp2D[j][i] * other.cmp[j];
            }
        }

    temp.x += cmp2D[3][0];
    temp.y += cmp2D[3][1];
    temp.z += cmp2D[3][2];

    return temp;
    }*/

matrix4x4 matrix4x4::multiply( JFLOAT other ) const
    {
    matrix4x4 temp;
    for( int i = 0; i < 16; i++ )
        {
        temp._data[i] = _data[i] * other;
        }
    return temp;
    }

/*matrix4x4 matrix4x4::operator *( JFLOAT other )
    {
    matrix4x4 temp;
    for( int i = 0; i < 16; i++ )
        {
        temp.cmp[i] = cmp[i] * other;
        }
    return temp;
    }*/

/*vectorX matrix4x4::operator *( vectorX other )
    {
    //std::cout<<other.size()<<std::endl;
    jAssert( other.size() == 4 );
    vectorX temp( 4 );
    for( int i = 0; i < 4; i++ )
        {
        for( int j = 0; j < 4; j++ )
            {
            temp[i] += cmp2D[j][i] * other[j];
            }
        }
    return temp;
    }*/

matrix4x4::matrix4x4()
    {
    }

const matrix4x4 matrix4x4::IDENTITY = matrix4x4::identity();

matrix4x4 matrix4x4::identity()
    {
    matrix4x4 ret;
    for( int x=0; x<4; x++ )
        {
        for( int y=0; y<4; y++ )
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

matrix4x3 matrix4x4::asMatrix4x3() const
    {
    matrix4x3 ret = matrix4x3::IDENTITY;

    for( unsigned int i=0; i<3; ++i )
        {
        for( unsigned int j=0; j<4; ++j )
            {
            ret.RM( i, j ) = RM( i, j );
            }
        }

    return ret;
    }

matrix3x3 matrix4x4::asMatrix3x3() const
    {
    return minor3x3( 3, 3 );
    }

std::ostream& operator<<( std::ostream& output, const matrix4x4 &other )
    {
    return output<<other.RM(0,0)<<" "<<other.RM(0,1)<<" "<<other.RM(0,2)<<" "<<other.RM(0,3)<<endl<<
                   other.RM(1,0)<<" "<<other.RM(1,1)<<" "<<other.RM(1,2)<<" "<<other.RM(1,3)<<endl<<
                   other.RM(2,0)<<" "<<other.RM(2,1)<<" "<<other.RM(2,2)<<" "<<other.RM(2,3)<<endl<<
                   other.RM(3,0)<<" "<<other.RM(3,1)<<" "<<other.RM(3,2)<<" "<<other.RM(3,3)<<endl;
    }

END_JLIB_MATH_NAMESPACE
