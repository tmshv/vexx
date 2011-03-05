#include "pVector.h"

BEGIN_PHYSICAL_NAMESPACE

pVector::pVector( pReal ass )
    {
    _size = 3;

    for( unsigned int x=0; x<_size; x++ )
        {
        cmp[x] = ass;
        }
    }

pVector::pVector( const jLib::math::triple &in )
    {
    _size = 3;

    cmp[0] = in.x();
    cmp[1] = in.y();
    cmp[2] = in.z();
    }

pVector::pVector( pReal x, pReal y, pReal z )
    {
    _size = 3;

    cmp[0] = x;
    cmp[1] = y;
    cmp[2] = z;
    }

pVector::pVector( const pVector &in )
    {
    jAssert( in._size <= 4 );
    _size = in._size;

    for( unsigned int x=0; x<_size; x++ )
        {
        cmp[x] = in.cmp[x];
        }
    }

//Vector Operations
pVector pVector::cross( const pVector &b ) const
    {
    return pVector( cmp[1]*b.cmp[2] - cmp[2]*b.cmp[1], cmp[2]*b.cmp[0] - cmp[0]*b.cmp[2], cmp[0]*b.cmp[1] - cmp[1]*b.cmp[0] );
    }

pReal pVector::dot( const pVector &in ) const
    {
    pReal ret=0;
    for( unsigned int x=0; x<_size; x++ )
        {
        ret += cmp[x] * in[x];
        }
    return ret;
    }

pReal pVector::lengthSquared() const
    {
    pReal ret=0;
    for( unsigned int x=0; x<_size; x++ )
        {
        ret += pow( cmp[x], 2 );
        }
    return ret;
    }

pReal pVector::length() const
    {
    return sqrt( lengthSquared() );
    }

pVector pVector::normalise() const
    {
    if( length() != 0 )
        {
        return (*this) / length();
        }
    return *this;
    }

pVector pVector::scale( pReal in ) const
    {
    return ( (*this) / length() ) * in;
    }

pVector pVector::scaleTo( pReal in ) const
    {
    return ( (*this) / length() ) * in;
    }

pVector pVector::negate() const
    {
    return (*this) * -1;
    }

pVector pVector::squared() const
    {
    pVector ret( *this );
    for( unsigned int x=0; x<_size; x++ )
        {
        ret.cmp[x] *= ret.cmp[x];
        }
    return ret;
    }

pVector pVector::sqroot() const
    {
    pVector ret( *this );
    for( unsigned int x=0; x<_size; x++ )
        {
        ret.cmp[x] = sqrt( ret.cmp[x] );
        }
    return ret;
    }

//Get Specific component
pReal& pVector::operator[]( unsigned int x )
    {
    jAssert( x < _size );
    return cmp[x];
    }

pReal pVector::operator[]( unsigned int x ) const
    {
    jAssert( x < _size );
    return cmp[x];
    }

//Equality
pVector &pVector::operator =( pReal in )
    {
    for( unsigned int x=0; x<_size; x++ )
        {
        cmp[x] = in;
        }
    return *this;
    }
JBOOL pVector::operator ==( pReal in ) const
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
JBOOL pVector::operator !=( pReal in ) const
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
pVector &pVector::operator =( pVector in )
    {
    jAssert( in._size == _size );
    for( unsigned int x=0; x<_size; x++ )
        {
        cmp[x] = in.cmp[x];
        }
    return *this;
    }
JBOOL pVector::operator ==( pVector in ) const
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
JBOOL pVector::operator !=( pVector in ) const
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

pVector pVector::operator -() const
    {
    return (*this) * -1;
    }

//Operate by vector on other
pVector pVector::operator +( pVector other ) const
    {
    jAssert( other._size == _size );
    pVector ret( *this );
    for( unsigned int x=0; x<_size; x++ )
        {
        ret.cmp[x] += other.cmp[x];
        }
    return ret;
    }
pVector pVector::operator -( pVector other ) const
    {
    jAssert( other._size == _size );
    pVector ret( *this );
    for( unsigned int x=0; x<_size; x++ )
        {
        ret.cmp[x] -= other.cmp[x];
        }
    return ret;
    }
pVector pVector::operator *( pVector other ) const
    {
    jAssert( other._size == _size );
    pVector ret( *this );
    for( unsigned int x=0; x<_size; x++ )
        {
        ret.cmp[x] *= other.cmp[x];
        }
    return ret;
    }
pVector pVector::operator /( pVector other ) const
    {
    jAssert( other._size == _size );
    pVector ret( *this );
    for( unsigned int x=0; x<_size; x++ )
        {
        ret.cmp[x] /= other.cmp[x];
        }
    return ret;
    }

//Operate by scalar on other
pVector pVector::operator /( pReal other ) const
    {
    pVector ret( *this );
    for( unsigned int x=0; x<_size; x++ )
        {
        ret.cmp[x] /= other;
        }
    return ret;
    }
pVector pVector::operator *( pReal other ) const
    {
    pVector ret( *this );
    for( unsigned int x=0; x<_size; x++ )
        {
        ret.cmp[x] *= other;
        }
    return ret;
    }
pVector pVector::operator +( pReal other ) const
    {
    pVector ret( *this );
    for( unsigned int x=0; x<_size; x++ )
        {
        ret.cmp[x] += other;
        }
    return ret;
    }
pVector pVector::operator -( pReal other ) const
    {
    pVector ret( *this );
    for( unsigned int x=0; x<_size; x++ )
        {
        ret.cmp[x] -= other;
        }
    return ret;
    }

//Operate by vector on self.
pVector pVector::operator +=( pVector other )
    {
    jAssert( other._size == _size );
    for( unsigned int x=0; x<_size; x++ )
        {
        cmp[x] += other.cmp[x];
        }
    return *this;
    }
pVector pVector::operator -=( pVector other )
    {
    jAssert( other._size == _size );
    for( unsigned int x=0; x<_size; x++ )
        {
        cmp[x] -= other.cmp[x];
        }
    return *this;
    }
pVector pVector::operator *=( pVector other )
    {
    jAssert( other._size == _size );
    for( unsigned int x=0; x<_size; x++ )
        {
        cmp[x] *= other.cmp[x];
        }
    return *this;
    }
pVector pVector::operator /=( pVector other )
    {
    jAssert( other._size == _size );
    for( unsigned int x=0; x<_size; x++ )
        {
        cmp[x] /= other.cmp[x];
        }
    return *this;
    }

//Operate by scalar on self.
pVector pVector::operator +=( pReal other )
    {
    for( unsigned int x=0; x<_size; x++ )
        {
        cmp[x] += other;
        }
    return *this;
    }
pVector pVector::operator -=( pReal other )
    {
    for( unsigned int x=0; x<_size; x++ )
        {
        cmp[x] -= other;
        }
    return *this;
    }
pVector pVector::operator *=( pReal other )
    {
    for( unsigned int x=0; x<_size; x++ )
        {
        cmp[x] *= other;
        }
    return *this;
    }
pVector pVector::operator /=( pReal other )
    {
    for( unsigned int x=0; x<_size; x++ )
        {
        cmp[x] /= other;
        }
    return *this;
    }

pVector operator/( const pReal & div, const pVector & in )
    {
    pVector ret( in );
    for( unsigned int x=0; x<in.size(); x++ )
        {
        ret[x] = div / ret[x];
        }
    return ret;
    }

pVector operator*( const pReal & mul, const pVector & in )
    {
    pVector ret( in );
    for( unsigned int x=0; x<in.size(); x++ )
        {
        ret[x] = mul * ret[x];
        }
    return ret;
    }

pVector operator+( const pReal & plus, const pVector & in )
    {
    pVector ret( in );
    for( unsigned int x=0; x<in.size(); x++ )
        {
        ret[x] = plus + ret[x];
        }
    return ret;
    }

pVector operator-( const pReal & sub, const pVector & in )
    {
    pVector ret( in );
    for( unsigned int x=0; x<in.size(); x++ )
        {
        ret[x] = sub * ret[x];
        }
    return ret;
    }

//Print Vector
std::ostream& operator<<(std::ostream& output, const pVector &other )
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
std::istream& operator>>(std::istream& input, pVector &other )
    {
    for( unsigned int x=0; x<other.size(); x++ )
        {
        input>>other[x];
        }
    return input;
    }

END_PHYSICAL_NAMESPACE
