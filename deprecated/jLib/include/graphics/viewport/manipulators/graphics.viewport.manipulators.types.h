#ifndef JLIB_GRAPHICS_VIEWPORT_MANIPULATORS_TYPES_H
#define JLIB_GRAPHICS_VIEWPORT_MANIPULATORS_TYPES_H

#include "graphics/viewport/graphics.viewport.types.h"

#define BEGIN_JLIB_GRAPHICS_VIEWPORT_MANIPULATORS_NAMESPACE             \
BEGIN_JLIB_GRAPHICS_VIEWPORT_NAMESPACE                                  \
namespace manipulators                                                  \
{

#define END_JLIB_GRAPHICS_VIEWPORT_MANIPULATORS_NAMESPACE               \
}                                                                       \
END_JLIB_GRAPHICS_VIEWPORT_NAMESPACE

BEGIN_JLIB_GRAPHICS_VIEWPORT_MANIPULATORS_NAMESPACE

class base;
class camera;
class selection;
class point;

END_JLIB_GRAPHICS_VIEWPORT_MANIPULATORS_NAMESPACE

#endif
