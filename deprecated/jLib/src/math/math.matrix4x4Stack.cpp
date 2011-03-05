#include "math/math.matrix4x4Stack.h"

BEGIN_JLIB_MATH_NAMESPACE

matrix4x4Stack::matrix4x4Stack( ) : _result( math::matrix4x4::IDENTITY )
    {
    }

matrix4x4Stack::~matrix4x4Stack( )
    {
    }

matrix4x4 &matrix4x4Stack::operator[]( unsigned int x )
    {
    jAssert( x < stack.size() );
    return stack[x];
    }

matrix4x4 &matrix4x4Stack::at( unsigned int x )
    {
    jAssert( x < stack.size() );
    return stack[x];
    }

const matrix4x4 &matrix4x4Stack::at( unsigned int x ) const
    {
    jAssert( x < stack.size() );
    return stack[x];
    }

void matrix4x4Stack::erase( unsigned int x )
    {
    jAssert( x < stack.size() );
    stack.erase( stack.begin() + x );
    }

unsigned int matrix4x4Stack::add( matrix4x4 mat )
    {
    stack.push_back( mat );
    return stack.size() - 1;
    }

unsigned int matrix4x4Stack::insert( unsigned int x, matrix4x4 mat )
    {
    stack.insert( stack.begin() + x, mat );
    return x;
    }

void matrix4x4Stack::swap( unsigned int a, unsigned int b )
    {
    jAssert( a < stack.size() );
    jAssert( b < stack.size() );
    matrix4x4 temp = stack[a];
    stack[a] = stack[b];
    stack[b] = stack[a];
    }

matrix4x4 matrix4x4Stack::resultant( ) const
    {
    return _result;
    }
void matrix4x4Stack::refresh( )
    {
    _result = matrix4x4::IDENTITY;

    for( unsigned int x=0; x<stack.size(); x++ )
        {
        //jDebugNeat<<x<<", A "<<_result<<endl<<"B "<<stack[x]<<endl;
        _result = _result * stack[x];
        }
    //jDebugNeat<<"Result: "<<_result<<endl;
    }

END_JLIB_MATH_NAMESPACE
