#ifndef JLIB_MATH_MATRIX4X4_H
#define JLIB_MATH_MATRIX4X4_H

#include "math/math.types.h"

BEGIN_JLIB_MATH_NAMESPACE

class JLIB_EXPORT matrix4x4
    {
public:
    //! Used to determine euler rotation matrix type
    //enum axis { X, Y, Z };

    //! Construct an identity matrix
    matrix4x4();
    //! Construct a matrix from a float pointer ( expects 16 values )
    //matrix4x4( JFLOAT *in );
    //! Construct from 16 floats
    /*matrix4x4( JFLOAT _a, JFLOAT _b, JFLOAT _c, JFLOAT _d,
        JFLOAT _e, JFLOAT _f, JFLOAT _g, JFLOAT _h,
        JFLOAT _i, JFLOAT _j, JFLOAT _k, JFLOAT _l,
        JFLOAT _m, JFLOAT _n, JFLOAT _o, JFLOAT _p );*/
    //! Get a pointer to the first elenet of the matrix
    const JFLOAT *ptr() const
        { return _data; }
    //! Get a pointer to the first elenet of the matrix
    //const JFLOAT *ptr() const;
    //! Get the current openGL model view matrix
    //matrix4x4 fillModelView();
    //! Get the current openGL projection matrix
    //matrix4x4 fillProjection();
    //! Multiply the current openGL matrix by this matrix
    void multiplyCurrent();

    //! Get the matrix as a 4x3 matrix ( disregards bottom row )
    matrix4x3 asMatrix4x3() const;
    //! Get the matrix as a 3x3 matrix ( disregards bottom and right row )
    matrix3x3 asMatrix3x3() const;

    //! Find the minor of this matrix
    matrix3x3 minor3x3( unsigned int i, unsigned int j ) const;
    //! Find the cofactor of this matrix
    matrix4x4 cofactor() const;
    //! Find the Transpose of this matrix
    matrix4x4 transpose() const;
    //! Find the *proper* inverse of this matrix ( works on all invertible matricies )
    matrix4x4 inverse() const;
    //! Find the quick inverse of this matrix ( works on orthogonal matricies )
    matrix4x4 fastInverse() const;
    //! Find the determinant of this matrix
    JFLOAT determinant() const;

    matrix4x4 multiply( const matrix4x4 & ) const;
    //! Multiple this matrix by another
    inline matrix4x4 operator *( const matrix4x4 & ) const;
    //! Multiply this matrix by a vector
    //vectorX operator *( vectorX other );
    //! Multiply this matrix by a vector structure
    //quickPt3 operator *( quickPt3 other ) const;
    //! Multiply this matrix by a float
    //matrix4x4 operator *( JFLOAT ) const;
    //! Multiply by a point ( optimised for transforms, expected homogeneous coords 0, 0, 0, 1 )
    triple pointMultiply( triple );
    //! Multiply by a vector ( optimised for transforms, expected homogeneous coords 0, 0, 0, 1 )
    triple vectorMultiply( triple );
    //! Test for equality
    bool isEqual( const matrix4x4 & ) const;
    //! test two matrixies for equality
    inline bool operator==( const matrix4x4 & ) const;
    //! divide each element of this matrix by a float
    matrix4x4 multiply( JFLOAT ) const;
    inline matrix4x4 operator*( JFLOAT ) const;
    inline matrix4x4 operator/( JFLOAT ) const;

    //! Get the normalised orientation part of the matrix ( assumes normalised input )
    quaternion orientation( ) const;
    //! Get the orientation part of the matrix ( assumes normalised input )
    void setOrientation( quaternion );

    //! Set the scale part of the matrix
    void setScale( const triple & );
    //! Get the scale part of the matrix
    triple scale( ) const;

    //! extract the translation elements from this matrix
    triple translation() const;
    //! set the translation elements for the matrix
    void setTranslation( triple );

    //! Access the X vector for the matrix
    triple xAxis( ) const;
    //! Access the Y vector for the matrix
    triple yAxis( ) const;
    //! Access the Z vector for the matrix
    triple zAxis( ) const;
    //! Access the X vector for the matrix
    void setXAxis( triple );
    //! Access the Y vector for the matrix
    void setYAxis( triple );
    //! Access the Z vector for the matrix
    void setZAxis( triple );

    //! Normalise the scale part of the matrix
    matrix4x4 normalise( ) const;
    //! Is the matrix's orientation already normalised
    bool isNormalised() const;

    //! generate a translation matrix
    //static matrix4x4 translate( triple );
    //! generate a euler rotation matrix
    //static matrix4x4 rotate( enum axis, float );
    //! generate a scale matrix
    //static matrix4x4 scale( triple );

    static matrix4x4 identity();
    const static matrix4x4 IDENTITY;

    // Access elements of the matrix
    inline JFLOAT &RM( unsigned int i, unsigned int j );
    inline JFLOAT &CM( unsigned int i, unsigned int j );
    inline JFLOAT RM( unsigned int i, unsigned int j ) const;
    inline JFLOAT CM( unsigned int i, unsigned int j ) const;
private:
    /*union
        {
        struct
            {
            JFLOAT a, b, c, d,
            e, f, g, h,
            i, j, k, l,
            m, n, o, p;
            };
        JFLOAT cmp2D[4][4];
        JFLOAT cmp[16];*/
        JFLOAT _data[16];
    //    };
    };

//! stream a matrix for debugging
std::ostream& JLIB_EXPORT operator<<(std::ostream& output, const matrix4x4 &other );

matrix4x4 matrix4x4::operator*( const matrix4x4 &in ) const
    {
    return multiply( in );
    }

bool matrix4x4::operator==( const matrix4x4 &in ) const
    {
    return isEqual( in );
    }

inline matrix4x4 matrix4x4::operator*( JFLOAT in ) const
    {
    return multiply( in );
    }

inline matrix4x4 matrix4x4::operator/( JFLOAT in ) const
    {
    return multiply( 1 / in );
    }

JFLOAT &matrix4x4::RM( unsigned int i, unsigned int j )
    {
    jAssert( i < 4 );
    jAssert( j < 4 );
    return _data[j*4+i];
    }
JFLOAT &matrix4x4::CM( unsigned int i, unsigned int j )
    {
    jAssert( i < 4 );
    jAssert( j < 4 );
    return _data[i*4+j];
    }
JFLOAT matrix4x4::RM( unsigned int i, unsigned int j ) const
    {
    jAssert( i < 4 );
    jAssert( j < 4 );
    return _data[j*4+i];
    }
JFLOAT matrix4x4::CM( unsigned int i, unsigned int j ) const
    {
    jAssert( i < 4 );
    jAssert( j < 4 );
    return _data[i*4+j];
    }

END_JLIB_MATH_NAMESPACE

#endif
