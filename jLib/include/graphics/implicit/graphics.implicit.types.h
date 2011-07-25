#ifndef JLIB_GRAPHICS_IMPLICIT_TYPES_H
#define JLIB_GRAPHICS_IMPLICIT_TYPES_H

#include "graphics/graphics.types.h"

#define BEGIN_JLIB_GRAPHICS_IMPLICIT_NAMESPACE          \
BEGIN_JLIB_GRAPHICS_NAMESPACE                           \
namespace implicit                                      \
{

#define END_JLIB_GRAPHICS_IMPLICIT_NAMESPACE            \
}                                                       \
END_JLIB_GRAPHICS_NAMESPACE

BEGIN_JLIB_GRAPHICS_IMPLICIT_NAMESPACE

class vector;
class vectorAxis;
class vectorPlane;

END_JLIB_GRAPHICS_IMPLICIT_NAMESPACE

#endif
