#ifndef JLIB_MATH_MATRIX3X3_H
#define JLIB_MATH_MATRIX3X3_H

#include "math/math.types.h"
#include "math/math.triple.h"
#include "QMatrix3x3"

BEGIN_JLIB_MATH_NAMESPACE

class JLIB_EXPORT matrix3x3
    {
public:
    //! Initiate to identity
    matrix3x3();
    //! Initiate fro Qt Type
    matrix3x3( QMatrix3x3 );

    const JFLOAT *ptr()
        { return _data; }

    //! Transpose this matrix
    matrix3x3 transpose() const;
    void setToTranspose();

    //! Get the inverse ( not optimised, actual inverse )
    matrix3x3 inverse() const;
    void setToInverse();

    //! Find the determinant of this matrix
    JFLOAT determinant() const;

    void setTranslate( const vector2 & );
    vector2 translate( );

    //! Set the scale part of the matrix
    void setScale( const triple & );
    //! Get the scale part of the matrix
    triple scale( ) const;
    //! Sets the orientation of this matrix
    void setOrientation( quaternion );
    //! Gets the orientation of this matrix
    quaternion orientation( ) const;

    //! Multiply by a matrix
    matrix3x3 multiply( const matrix3x3 & ) const;
    //! Multiply by a triple ( no homogeneous elements )
    triple multiply( const triple & ) const;

    //! Test if two matrixes are equal
    inline bool operator==( const matrix3x3 & ) const;
    //! Multiply point by this matrix
    inline triple operator*( const triple & );
    //! Multiply matrix by this matrix
    inline matrix3x3 operator*( const matrix3x3 & );
    //! Test if two matrixes are equal
    bool isEqual( const matrix3x3 & ) const;

    // Access elements of the matrix
    inline JFLOAT &RM( unsigned int i, unsigned int j );
    inline JFLOAT &CM( unsigned int i, unsigned int j );
    inline JFLOAT RM( unsigned int i, unsigned int j ) const;
    inline JFLOAT CM( unsigned int i, unsigned int j ) const;

    static matrix3x3 identity();
    const static matrix3x3 IDENTITY;

private:
    JFLOAT _data[9];
    friend class matrix4x4;
    };

triple matrix3x3::operator*( const triple &b )
    {
    return multiply( b );
    }


matrix3x3 matrix3x3::operator*( const matrix3x3 &b )
    {
    return multiply( b );
    }

bool matrix3x3::operator==( const matrix3x3 &in ) const
    {
    return isEqual( in );
    }

std::ostream& JLIB_EXPORT operator<<(std::ostream& output, matrix3x3 other );

JFLOAT &matrix3x3::RM( unsigned int i, unsigned int j )
    {
    jAssert( i < 3 );
    jAssert( j < 3 );
    return _data[j*3+i];
    }
JFLOAT &matrix3x3::CM( unsigned int i, unsigned int j )
    {
    jAssert( i < 3 );
    jAssert( j < 3 );
    return _data[i*3+j];
    }
JFLOAT matrix3x3::RM( unsigned int i, unsigned int j ) const
    {
    jAssert( i < 3 );
    jAssert( j < 3 );
    return _data[j*3+i];
    }
JFLOAT matrix3x3::CM( unsigned int i, unsigned int j ) const
    {
    jAssert( i < 3 );
    jAssert( j < 3 );
    return _data[i*3+j];
    }

END_JLIB_MATH_NAMESPACE

#endif
