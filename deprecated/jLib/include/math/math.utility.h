#ifndef JLIB_MATH_UTILITY_H
#define JLIB_MATH_UTILITY_H

BEGIN_JLIB_MATH_NAMESPACE

// The accuracy of floating point comparison ( fcmp )
#define EPSILON 0.001f

// The floating point comparison function
inline bool fcmp( JFLOAT a, JFLOAT b )
    {
    if( (a-EPSILON)<b && (a+EPSILON)>b )
        {
        return TRUE;
        }
    else
        {
        return FALSE;
        }
    }

// The floating point comparison function
inline bool fcmp( JFLOAT a, JFLOAT b, JFLOAT eps )
    {
    if( (a-eps)<b && (a+eps)>b )
        {
        return TRUE;
        }
    else
        {
        return FALSE;
        }
    }

inline JFLOAT clamp( JFLOAT in, JFLOAT min, JFLOAT max )
    {
    if( in < min )
        {
        return min;
        }
    else if( in > max )
        {
        return max;
        }
    return in;
    }

inline unsigned int factorial( unsigned int in )
    {
    // multiply 1 * 2 * 3 * ... * in
    unsigned int ret = 1;
    for( unsigned int x=2; x<=in; x++ )
        {
        ret *= x;
        }
    return ret;
    }

inline unsigned int binomialCoefficient( unsigned n, unsigned k )
    {
    // Find the coefficient of n and k...
    return factorial( n ) / ( ( factorial( n - k ) * factorial( k ) ) );
    }

END_JLIB_MATH_NAMESPACE

#endif
