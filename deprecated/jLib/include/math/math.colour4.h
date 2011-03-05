#ifndef JLIB_MATH_COLOUR4_H
#define JLIB_MATH_COLOUR4_H

#include "math/math.types.h"
#include "math/math.vectorX.h"
#include "math/math.random.h"
#include "QVector4D"

BEGIN_JLIB_MATH_NAMESPACE

//! Holds a 4 dimensional colour
class JLIB_EXPORT colour4 : public vectorX
    {
public:
    //! Get Red Component
    inline JFLOAT& r()
        { return cmp[0]; }
    //! Get Green Component
    inline JFLOAT& g()
        { return cmp[1]; }
    //! Get Blue Component
    inline JFLOAT& b()
        { return cmp[2]; }
    //! Get Alpha Component
    inline JFLOAT& a()
        { return cmp[3]; }

    //! Init to White
    colour4( );
    //! Init from vector4D
    colour4( QVector4D );
    //! Init to A Gray
    colour4( float _l );
    //! Init with RGBA Components
    colour4( float _r, float _g, float _b, float _a );
    //! Init with RGB Components
    colour4( float _r, float _g, float _b );
    //! Colour froma root object ( useful... )
    colour4( vectorX in );
    //! Clone a colour
    colour4( const colour4 &nv );
    //! Sets the current openGL colour to this
    void load();

    static colour4 random( bool a=FALSE );

    const static colour4 BLACK;
    const static colour4 WHITE;
    const static colour4 RED;
    const static colour4 GREEN;
    const static colour4 BLUE;
    const static colour4 YELLOW;
    const static colour4 MAGENTA;
    const static colour4 CYAN;
    };

END_JLIB_MATH_NAMESPACE

#endif
