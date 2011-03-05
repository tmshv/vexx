#ifndef JLIB_MATH_VECTOR2_H
#define JLIB_MATH_VECTOR2_H

#include "math/math.types.h"
#include "math/math.vectorX.h"

BEGIN_JLIB_MATH_NAMESPACE

class JLIB_EXPORT vector2 : public vectorX
    {
public:
    inline JFLOAT& x()
        { return cmp[0]; }
    inline JFLOAT& y()
        { return cmp[1]; }
    inline const JFLOAT& x() const
        { return cmp[0]; }
    inline const JFLOAT& y() const
        { return cmp[1]; }

    vector2( );
    vector2( float in );
    vector2( float X, float Y );
    vector2( const vector2 &nv );
    vector2( vectorX in );
    };

END_JLIB_MATH_NAMESPACE

#endif
