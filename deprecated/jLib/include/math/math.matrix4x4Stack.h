#ifndef JLIB_MATH_MATRIX4X4STACK_H
#define JLIB_MATH_MATRIX4X4STACK_H

#include "math/math.types.h"
#include "math/math.matrix4x4.h"

BEGIN_JLIB_MATH_NAMESPACE

class matrix4x4Stack
    {
public:
    matrix4x4Stack( );
    virtual ~matrix4x4Stack( );
    matrix4x4 &operator[]( unsigned int );
    matrix4x4 &at( unsigned int );
    const matrix4x4 &at( unsigned int ) const;
    void erase( unsigned int );
    unsigned int add( matrix4x4 );
    unsigned int insert( unsigned int, matrix4x4 );
    void swap( unsigned int, unsigned int );
    matrix4x4 resultant( ) const;
    virtual void refresh( );
private:
    matrix4x4 _result;
    vector <matrix4x4> stack;
    };

END_JLIB_MATH_NAMESPACE

#endif
