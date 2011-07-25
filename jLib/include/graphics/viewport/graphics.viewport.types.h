#ifndef JLIB_GRAPHICS_VIEWPORT_TYPES_H
#define JLIB_GRAPHICS_VIEWPORT_TYPES_H

#include "graphics/graphics.types.h"

#define BEGIN_JLIB_GRAPHICS_VIEWPORT_NAMESPACE          \
BEGIN_JLIB_GRAPHICS_NAMESPACE                           \
namespace viewport                                      \
{

#define END_JLIB_GRAPHICS_VIEWPORT_NAMESPACE        \
}                                                   \
END_JLIB_GRAPHICS_NAMESPACE

BEGIN_JLIB_GRAPHICS_VIEWPORT_NAMESPACE

class manipulatorManager;

END_JLIB_GRAPHICS_VIEWPORT_NAMESPACE

#endif
