#include "math/math.vectorX.h"

BEGIN_JLIB_MATH_NAMESPACE

vectorX::vectorX( unsigned int s )
    {
    jAssert( s <= 4 );
    _size = s;

    for( unsigned int x=0; x<_size; x++ )
        {
        cmp[x] = 0;
        }
    }
vectorX::vectorX( unsigned int s, JFLOAT *in )
    {
    jAssert( s <= 4 );
    _size = s;

    for( unsigned int x=0; x<_size; x++ )
        {
        cmp[x] = in[x];
        }
    }
vectorX::vectorX( unsigned int s, JFLOAT in )
    {
    jAssert( s <= 4 );
    _size = s;

    for( unsigned int x=0; x<_size; x++ )
        {
        cmp[x] = in;
        }
    }
vectorX::vectorX( JFLOAT x, JFLOAT y, JFLOAT z )
    {
    _size = 3;

    cmp[0] = x;
    cmp[1] = y;
    cmp[2] = z;
    }
vectorX::vectorX( JFLOAT x, JFLOAT y, JFLOAT z, JFLOAT w )
    {
    _size = 4;

    cmp[0] = x;
    cmp[1] = y;
    cmp[2] = z;
    cmp[3] = w;
    }
vectorX::vectorX( const vectorX &in )
    {
    jAssert( in._size <= 4 );
    _size = in._size;

    for( unsigned int x=0; x<_size; x++ )
        {
        cmp[x] = in.cmp[x];
        }
    }
vectorX::vectorX( vectorX &in )
    {
    jAssert( in._size <= 4 );
    _size = in._size;

    for( unsigned int x=0; x<_size; x++ )
        {
        cmp[x] = in.cmp[x];
        }
    }
vectorX::~vectorX()
    {
    }

//Vector Operations
JFLOAT vectorX::lengthSquared() const
    {
    JFLOAT ret=0;
    for( unsigned int x=0; x<_size; x++ )
        {
        ret += pow( cmp[x], 2 );
        }
    return ret;
    }

JFLOAT vectorX::length() const
    {
    return sqrt( lengthSquared() );
    }

vectorX vectorX::normalise() const
    {
    if( length() != 0 )
        {
        return (*this) / length();
        }
    return *this;
    }

vectorX vectorX::scale( JFLOAT in ) const
    {
    return ( (*this) / length() ) * in;
    }

vectorX vectorX::scaleTo( JFLOAT in ) const
    {
    return ( (*this) / length() ) * in;
    }

vectorX vectorX::negate() const
    {
    return (*this) * -1;
    }

vectorX vectorX::squared() const
    {
    vectorX ret( *this );
    for( unsigned int x=0; x<_size; x++ )
        {
        ret.cmp[x] *= ret.cmp[x];
        }
    return ret;
    }

vectorX vectorX::sqroot() const
    {
    vectorX ret( *this );
    for( unsigned int x=0; x<_size; x++ )
        {
        ret.cmp[x] = sqrt( ret.cmp[x] );
        }
    return ret;
    }

//Get Specific component
JFLOAT& vectorX::operator[]( unsigned int x )
    {
    jAssert( x < _size );
    return cmp[x];
    }

JFLOAT vectorX::operator[]( unsigned int x ) const
    {
    jAssert( x < _size );
    return cmp[x];
    }

//Equality
void vectorX::operator =( JFLOAT in )
    {
    for( unsigned int x=0; x<_size; x++ )
        {
        cmp[x] = in;
        }
    }
JBOOL vectorX::operator ==( JFLOAT in ) const
    {
    for( unsigned int x=0; x<_size; x++ )
        {
        if( !fcmp( cmp[x], in ) )
            {
            return FALSE;
            }
        }
    return TRUE;
    }
JBOOL vectorX::operator !=( JFLOAT in ) const
    {
    unsigned int ret = 0;
    for( unsigned int x=0; x<_size; x++ )
        {
        if( !fcmp( cmp[x], in ) )
            {
            ret++;
            }
        }
    return ( ret>0 );
    }
void vectorX::operator =( vectorX in )
    {
    jAssert( in._size == _size );
    for( unsigned int x=0; x<_size; x++ )
        {
        cmp[x] = in.cmp[x];
        }
    }
JBOOL vectorX::operator ==( vectorX in ) const
    {
    jAssert( in._size == _size );
    for( unsigned int x=0; x<_size; x++ )
        {
        if( !fcmp( cmp[x], in.cmp[x] ) )
            {
            return FALSE;
            }
        }
    return TRUE;
    }
JBOOL vectorX::operator !=( vectorX in ) const
    {
    jAssert( in._size == _size );
    for( unsigned int x=0; x<_size; x++ )
        {
        if( !fcmp( cmp[x], in.cmp[x] ) )
            {
            return TRUE;
            }
        }
    return FALSE;
    }

vectorX vectorX::operator -() const
    {
    return (*this) * -1;
    }

//Operate by vector on other
vectorX vectorX::operator +( vectorX other ) const
    {
    jAssert( other._size == _size );
    vectorX ret( *this );
    for( unsigned int x=0; x<_size; x++ )
        {
        ret.cmp[x] += other.cmp[x];
        }
    return ret;
    }
vectorX vectorX::operator -( vectorX other ) const
    {
    jAssert( other._size == _size );
    vectorX ret( *this );
    for( unsigned int x=0; x<_size; x++ )
        {
        ret.cmp[x] -= other.cmp[x];
        }
    return ret;
    }
vectorX vectorX::operator *( vectorX other ) const
    {
    jAssert( other._size == _size );
    vectorX ret( *this );
    for( unsigned int x=0; x<_size; x++ )
        {
        ret.cmp[x] *= other.cmp[x];
        }
    return ret;
    }
vectorX vectorX::operator /( vectorX other ) const
    {
    jAssert( other._size == _size );
    vectorX ret( *this );
    for( unsigned int x=0; x<_size; x++ )
        {
        ret.cmp[x] /= other.cmp[x];
        }
    return ret;
    }

//Operate by scalar on other
vectorX vectorX::operator /( JFLOAT other ) const
    {
    vectorX ret( *this );
    for( unsigned int x=0; x<_size; x++ )
        {
        ret.cmp[x] /= other;
        }
    return ret;
    }
vectorX vectorX::operator *( JFLOAT other ) const
    {
    vectorX ret( *this );
    for( unsigned int x=0; x<_size; x++ )
        {
        ret.cmp[x] *= other;
        }
    return ret;
    }
vectorX vectorX::operator +( JFLOAT other ) const
    {
    vectorX ret( *this );
    for( unsigned int x=0; x<_size; x++ )
        {
        ret.cmp[x] += other;
        }
    return ret;
    }
vectorX vectorX::operator -( JFLOAT other ) const
    {
    vectorX ret( *this );
    for( unsigned int x=0; x<_size; x++ )
        {
        ret.cmp[x] -= other;
        }
    return ret;
    }

//Operate by vector on self.
vectorX vectorX::operator +=( vectorX other )
    {
    jAssert( other._size == _size );
    for( unsigned int x=0; x<_size; x++ )
        {
        cmp[x] += other.cmp[x];
        }
    return *this;
    }
vectorX vectorX::operator -=( vectorX other )
    {
    jAssert( other._size == _size );
    for( unsigned int x=0; x<_size; x++ )
        {
        cmp[x] -= other.cmp[x];
        }
    return *this;
    }
vectorX vectorX::operator *=( vectorX other )
    {
    jAssert( other._size == _size );
    for( unsigned int x=0; x<_size; x++ )
        {
        cmp[x] *= other.cmp[x];
        }
    return *this;
    }
vectorX vectorX::operator /=( vectorX other )
    {
    jAssert( other._size == _size );
    for( unsigned int x=0; x<_size; x++ )
        {
        cmp[x] /= other.cmp[x];
        }
    return *this;
    }

//Operate by scalar on self.
vectorX vectorX::operator +=( JFLOAT other )
    {
    for( unsigned int x=0; x<_size; x++ )
        {
        cmp[x] += other;
        }
    return *this;
    }
vectorX vectorX::operator -=( JFLOAT other )
    {
    for( unsigned int x=0; x<_size; x++ )
        {
        cmp[x] -= other;
        }
    return *this;
    }
vectorX vectorX::operator *=( JFLOAT other )
    {
    for( unsigned int x=0; x<_size; x++ )
        {
        cmp[x] *= other;
        }
    return *this;
    }
vectorX vectorX::operator /=( JFLOAT other )
    {
    for( unsigned int x=0; x<_size; x++ )
        {
        cmp[x] /= other;
        }
    return *this;
    }

vectorX operator/( const JFLOAT & div, const vectorX & in )
    {
    vectorX ret( in );
    for( unsigned int x=0; x<in.size(); x++ )
        {
        ret[x] = div / ret[x];
        }
    return ret;
    }

vectorX operator*( const JFLOAT & mul, const vectorX & in )
    {
    vectorX ret( in );
    for( unsigned int x=0; x<in.size(); x++ )
        {
        ret[x] = mul * ret[x];
        }
    return ret;
    }

vectorX operator+( const JFLOAT & plus, const vectorX & in )
    {
    vectorX ret( in );
    for( unsigned int x=0; x<in.size(); x++ )
        {
        ret[x] = plus + ret[x];
        }
    return ret;
    }

vectorX operator-( const JFLOAT & sub, const vectorX & in )
    {
    vectorX ret( in );
    for( unsigned int x=0; x<in.size(); x++ )
        {
        ret[x] = sub * ret[x];
        }
    return ret;
    }

//Print Vector
std::ostream& operator<<(std::ostream& output, const vectorX &other )
    {
    for( unsigned int x=0; x<other.size(); x++ )
        {
        output<<other[x];
        if( (x+1) < other.size() )
            {
            output<<" ";
            }
        }
    return output;
    }
std::istream& operator>>(std::istream& input, vectorX &other )
    {
    for( unsigned int x=0; x<other.size(); x++ )
        {
        input>>other[x];
        }
    return input;
    }

END_JLIB_MATH_NAMESPACE
