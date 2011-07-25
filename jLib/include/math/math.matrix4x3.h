#ifndef JLIB_MATH_MATRIX4X3_H
#define JLIB_MATH_MATRIX4X3_H

#include "math/math.types.h"
#include "math/math.quaternion.h"

BEGIN_JLIB_MATH_NAMESPACE

class JLIB_EXPORT matrix4x3
    {
public:
    matrix4x3();
    matrix4x3( triple, quaternion=quaternion(), triple=triple() );

    const JFLOAT *ptr()
        { return _data; }

    // Set data in the matrix
    void setOrientation( quaternion );
    void setScale( triple );
    void setTranslation( triple );

    // Access data in the matrix
    triple scale() const;
    triple translation() const;
    quaternion orientation() const;

    // Inverse functions
    matrix4x3 inverse() const;
    void setToInverse();

    // Transpose functions
    matrix4x3 transpose() const;
    void setToTranspose();

    // Multiply functions
    triple pointMultiply( const triple & ) const;
    triple vectorMultiply( const triple & ) const;
    matrix4x3 multiply( const matrix4x3 & ) const;
    matrix3x3 multiply( const matrix3x3 & ) const;

    //! Get the orientation / scale components
    matrix3x3 asMatrix3x3() const;
    //! Get as a 4x4 matrix for use in functions
    matrix4x4 asMatrix4x4() const;
    //! Test if two matrixes are equal
    inline bool operator==( const matrix4x3 & );
    //! Test if two matrixes are equal
    bool isEqual( const matrix4x3 & );

    // Access elements of the matrix
    inline JFLOAT &RM( unsigned int i, unsigned int j );
    inline JFLOAT &CM( unsigned int i, unsigned int j );
    inline JFLOAT RM( unsigned int i, unsigned int j ) const;
    inline JFLOAT CM( unsigned int i, unsigned int j ) const;

    static matrix4x3 identity();
    const static matrix4x3 IDENTITY;

protected:
    inline JFLOAT &_RM( unsigned int i, unsigned int j );
    inline JFLOAT &_CM( unsigned int i, unsigned int j );
    //! Set the matrix's homogeneous elements
    inline void setHomogeneous();
private:
    //! Holds the matrix's internal data
    JFLOAT _data[16];
    };

std::ostream& JLIB_EXPORT operator<<(std::ostream&, const matrix4x3 & );

bool matrix4x3::operator==( const matrix4x3 &in )
    {
    return isEqual( in );
    }

JFLOAT &matrix4x3::RM( unsigned int i, unsigned int j )
    {
    jAssert( i < 3 );
    jAssert( j < 4 );
    return _data[j*4+i];
    }
JFLOAT &matrix4x3::CM( unsigned int i, unsigned int j )
    {
    jAssert( i < 4 );
    jAssert( j < 3 );
    return _data[i*4+j];
    }
JFLOAT matrix4x3::RM( unsigned int i, unsigned int j ) const
    {
    jAssert( i < 4 );
    jAssert( j < 4 );
    return _data[j*4+i];
    }
JFLOAT matrix4x3::CM( unsigned int i, unsigned int j ) const
    {
    jAssert( i < 4 );
    jAssert( j < 4 );
    return _data[i*4+j];
    }

JFLOAT &matrix4x3::_RM( unsigned int i, unsigned int j )
    {
    jAssert( i < 4 );
    jAssert( j < 4 );
    return _data[j*4+i];
    }
JFLOAT &matrix4x3::_CM( unsigned int i, unsigned int j )
    {
    jAssert( i < 4 );
    jAssert( j < 4 );
    return _data[i*4+j];
    }
void matrix4x3::setHomogeneous()
    {
    _RM(0,3) = 0;
    _RM(1,3) = 0;
    _RM(2,3) = 0;
    _RM(3,3) = 1;
    }

END_JLIB_MATH_NAMESPACE

#endif
