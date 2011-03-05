#ifndef JLIB_GRPAHICS_TYPES_H
#define JLIB_GRAPHICS_TYPES_H

//Global
#include "jLib.global.h"
#include "math/math.h"
#include "file/file.h"
#include "error/error.h"
#include "jLib.jptr.h"

#define BEGIN_JLIB_GRAPHICS_NAMESPACE       \
BEGIN_JLIB_NAMESPACE                        \
namespace graphics                          \
{

#define END_JLIB_GRAPHICS_NAMESPACE         \
}                                           \
END_JLIB_NAMESPACE

//Just in case, because i had some weird experiences once...
#ifndef false
#   define false 0
#endif
#ifndef true
#   define true (!(false))
#endif

//Because the capital letters please me.
#ifndef FALSE
#   define FALSE false
#endif

#ifndef TRUE
#   define TRUE (!(false))
#endif

BEGIN_JLIB_GRAPHICS_NAMESPACE

namespace render
{
class scene;
}
END_JLIB_GRAPHICS_NAMESPACE

#endif
