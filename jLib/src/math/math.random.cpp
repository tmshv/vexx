#include "math/math.random.h"
#include "time.h"

BEGIN_JLIB_MATH_NAMESPACE

void random::ensureSeeded( )
    {
    static bool seeded = FALSE;
    if( seeded == FALSE )
        {
        srand( time( 0 ) );
        seeded = TRUE;
        }
    }
JFLOAT random::getF( JFLOAT min, JFLOAT max )
    {
    ensureSeeded();
    return min + ( ( (float)rand() / RAND_MAX ) * ( max - min ) );
    }
JINT random::getI( JINT min, JINT max )
    {
    ensureSeeded();
    return min + ( rand() % ( 1 + max - min ) );
    }

END_JLIB_MATH_NAMESPACE
