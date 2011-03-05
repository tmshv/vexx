#include "math/math.sseTriple.h"

#ifdef __SSE__

BEGIN_JLIB_MATH_NAMESPACE

void *sseTriple::operator new( size_t s )
    {
    jFail( "This is silly, write this function." );
    return malloc( s );
    }

void sseTriple::operator delete( void *in )
    {
    jFail( "This is silly, write this function." );
    free( in );
    }

END_JLIB_MATH_NAMESPACE

#endif
