#ifndef JLIB_MATH_POLAR_H
#define JLIB_MATH_POLAR_H

#include "math/math.types.h"
#include "math/math.triple.h"

BEGIN_JLIB_MATH_NAMESPACE

class polar
    {
public:
    inline JFLOAT &phi()
        { return cmp[0]; }
    inline JFLOAT &theta()
        { return cmp[1]; }
    inline JFLOAT &radius()
        { return cmp[2]; }

    polar();
    polar( const triple & );

    triple getPoint();
private:
    JFLOAT cmp[4];
    };

END_JLIB_MATH_NAMESPACE

#endif
