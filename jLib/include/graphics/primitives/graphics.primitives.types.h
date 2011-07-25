#ifndef JLIB_GRAPHICS_PRIMITIVES_TYPES_H
#define JLIB_GRAPHICS_PRIMITIVES_TYPES_H

#include "graphics/graphics.types.h"

#define BEGIN_JLIB_GRAPHICS_PRIMITIVES_NAMESPACE    \
BEGIN_JLIB_GRAPHICS_NAMESPACE                       \
namespace primitives                                    \
{

#define END_JLIB_GRAPHICS_PRIMITIVES_NAMESPACE      \
}                                                   \
END_JLIB_GRAPHICS_NAMESPACE

BEGIN_JLIB_GRAPHICS_PRIMITIVES_NAMESPACE

class pointArray;
class pointCloud;
class lineArray;
class lineCloud;
class triangleArray;
class triangleCloud;

END_JLIB_GRAPHICS_PRIMITIVES_NAMESPACE

#endif
