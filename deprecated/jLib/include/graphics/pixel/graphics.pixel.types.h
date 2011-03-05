#ifndef JLIB_GRAPHICS_PIXEL_TYPES_H
#define JLIB_GRAPHICS_PIXEL_TYPES_H

#include "graphics/graphics.types.h"

#define BEGIN_JLIB_GRAPHICS_PIXEL_NAMESPACE         \
BEGIN_JLIB_GRAPHICS_NAMESPACE                       \
namespace pixel                                     \
{

#define END_JLIB_GRAPHICS_PIXEL_NAMESPACE           \
}                                                   \
END_JLIB_GRAPHICS_NAMESPACE

BEGIN_JLIB_GRAPHICS_PIXEL_NAMESPACE

class image;
class imagePlane;
class textLine;
class texture;
class texture3;
class texture4;

END_JLIB_GRAPHICS_PIXEL_NAMESPACE

#endif
