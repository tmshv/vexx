#ifndef JLIB_GRAPHICS_RENDER_TYPES_H
#define JLIB_GRAPHICS_RENDER_TYPES_H

#include "graphics/graphics.types.h"

#define BEGIN_JLIB_GRAPHICS_RENDER_NAMESPACE    \
BEGIN_JLIB_GRAPHICS_NAMESPACE                       \
namespace render                                    \
{

#define END_JLIB_GRAPHICS_RENDER_NAMESPACE      \
}                                                   \
END_JLIB_GRAPHICS_NAMESPACE

BEGIN_JLIB_GRAPHICS_RENDER_NAMESPACE

class frameBuffer;
class scene;
class target;

END_JLIB_GRAPHICS_RENDER_NAMESPACE

#endif
