#ifndef JLIB_FLOATLIMITS_H
#define JLIB_FLOATLIMITS_H

#include "jLib.global.h"

BEGIN_JLIB_NAMESPACE

class JLIB_EXPORT floatLimits
    {
public:
    inline static JFLOAT infinity()
        {
        return std::numeric_limits<JFLOAT>::infinity();
        }
    inline static JFLOAT nan()
        {
        return std::numeric_limits<JFLOAT>::quiet_NaN();
        }
    };

END_JLIB_NAMESPACE

#endif
