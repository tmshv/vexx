#ifndef MATH_H
#define MATH_H

#include "math/math.types.h"

BEGIN_JLIB_MATH_NAMESPACE

class JLIB_EXPORT random
    {
public:
    static unsigned int seed;
    static void ensureSeeded( );
    static JFLOAT getF( JFLOAT, JFLOAT );
    static JINT getI( JINT, JINT );
};

END_JLIB_MATH_NAMESPACE

#endif // MATH_H
