#ifndef JLIB_MATH_TYPES_H
#define JLIB_MATH_TYPES_H

#include "jLib.global.h"
#include "error/error.h"

// Converting from degrees to radians
#ifndef DEGTORAD
#   define DEGTORAD 0.017453293
#endif

// Converting from radians to degrees
#ifndef RADTODEG
#   define RADTODEG 57.295779506
#endif

// Because windows doesn't declare M_PI
#ifndef M_PI
#   define M_PI 3.14159265358979323846
#endif

// Rather than multiplying M_PI by two all the time.
#ifndef JM2xPI
#   define JM2xPI 6.283185308
#endif

#define BEGIN_JLIB_MATH_NAMESPACE       \
BEGIN_JLIB_NAMESPACE                    \
namespace math                          \
{

#define END_JLIB_MATH_NAMESPACE         \
}                                       \
END_JLIB_NAMESPACE

BEGIN_JLIB_MATH_NAMESPACE

typedef union
    {
    struct { JFLOAT x, y, z; };
    struct { JFLOAT r, g, b; };
    struct { JFLOAT u, v, s; };
    float cmp[3];
    } quickPt3;

typedef union
    {
    struct { JFLOAT x, y, z, w; };
    struct { JFLOAT r, g, b, a; };
    struct { JFLOAT u, v, s, t; };
    float cmp[4];
    } quickPt4;

typedef union
    {
    struct { JFLOAT u, v; };
    struct { JFLOAT x, y; };
    JFLOAT cmp[2];
    } quickPt2;

typedef enum { ROTATEX, ROTATEY, ROTATEZ, TRANSLATEXYZ } matrixType;

/*typedef union
    {
    struct { JFLOAT r, phi, theta; };
    JFLOAT points[3];
    } polar3;*/

class vector2;
class triple;
class colour4;
class vectorX;
class matrix4x4;
class matrix4x3;
class matrix3x3;
class quaternion;

END_JLIB_MATH_NAMESPACE

#include "math/math.utility.h"

#endif
