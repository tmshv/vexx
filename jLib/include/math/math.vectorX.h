#ifndef JLIB_MATH_VECTORX_H
#define JLIB_MATH_VECTORX_H

#include "math/math.types.h"

BEGIN_JLIB_MATH_NAMESPACE

class JLIB_EXPORT vectorX
    {
public:
    //Initiators
    explicit vectorX( unsigned int s );
    explicit vectorX( unsigned int s, JFLOAT *in );
    explicit vectorX( unsigned int s, JFLOAT in );
    vectorX( const vectorX &in );
    vectorX( vectorX & );
    vectorX( JFLOAT x, JFLOAT y, JFLOAT z );
    vectorX( JFLOAT x, JFLOAT y, JFLOAT z, JFLOAT w );
    ~vectorX();

    //Vector Operations
    JFLOAT length() const;
    JFLOAT lengthSquared() const;
    vectorX normalise() const;
    vectorX scale( JFLOAT in ) const;
    vectorX scaleTo( JFLOAT in ) const;
    vectorX negate() const;
    vectorX squared() const;
    vectorX sqroot() const;

    //Access Methods
    JFLOAT& operator[]( unsigned int x );
    JFLOAT operator[]( unsigned int x ) const;
    inline JFLOAT *ptr()
        { return cmp; }
    inline unsigned int size() const
        { return _size; }

    //Equality
    void operator =( JFLOAT in );
    JBOOL operator ==( JFLOAT in ) const;
    JBOOL operator !=( JFLOAT in ) const;
    void operator =( vectorX in );
    JBOOL operator ==( vectorX in ) const;
    JBOOL operator !=( vectorX in ) const;

    //negate the vector
    vectorX operator -() const;

    //Operate by vector on other
    vectorX operator +( vectorX other ) const;
    vectorX operator -( vectorX other ) const;
    vectorX operator *( vectorX other ) const;
    vectorX operator /( vectorX other ) const;

    //Operate by scalar on other
    vectorX operator /( JFLOAT other ) const;
    vectorX operator *( JFLOAT other ) const;
    vectorX operator +( JFLOAT other ) const;
    vectorX operator -( JFLOAT other ) const;

    //Operate by vector on self.
    vectorX operator +=( vectorX other );
    vectorX operator -=( vectorX other );
    vectorX operator *=( vectorX other );
    vectorX operator /=( vectorX other );

    //Operate by scalar on self.
    vectorX operator +=( JFLOAT other );
    vectorX operator -=( JFLOAT other );
    vectorX operator *=( JFLOAT other );
    vectorX operator /=( JFLOAT other );

protected:
    unsigned int _size;
    JFLOAT cmp[4];
    };

std::ostream& JLIB_EXPORT operator<<(std::ostream& output, const vectorX &other );
std::istream& JLIB_EXPORT operator>>(std::istream& input, vectorX &ther );

vectorX JLIB_EXPORT operator/( const JFLOAT &, const vectorX & );
vectorX JLIB_EXPORT operator*( const JFLOAT &, const vectorX & );
vectorX JLIB_EXPORT operator+( const JFLOAT &, const vectorX & );
vectorX JLIB_EXPORT operator-( const JFLOAT &, const vectorX & );

END_JLIB_MATH_NAMESPACE

#endif
