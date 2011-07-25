#ifndef JLIB_GRAPHICS_SHAPE_TYPES_H
#define JLIB_GRAPHICS_SHAPE_TYPES_H

#include "graphics/graphics.types.h"

#define BEGIN_JLIB_GRAPHICS_SHAPE_NAMESPACE         \
BEGIN_JLIB_GRAPHICS_NAMESPACE                       \
namespace shape                                     \
{

#define END_JLIB_GRAPHICS_SHAPE_NAMESPACE           \
}                                                   \
END_JLIB_GRAPHICS_NAMESPACE

BEGIN_JLIB_GRAPHICS_SHAPE_NAMESPACE

class box;
class sphere;
class line;
class triangle;

END_JLIB_GRAPHICS_SHAPE_NAMESPACE

#endif
