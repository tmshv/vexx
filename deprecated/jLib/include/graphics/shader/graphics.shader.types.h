#ifndef JLIB_GRAPHICS_SHADER_TYPES_H
#define JLIB_GRAPHICS_SHADER_TYPES_H

#include "graphics/graphics.types.h"

#define BEGIN_JLIB_GRAPHICS_SHADER_NAMESPACE    \
BEGIN_JLIB_GRAPHICS_NAMESPACE                       \
namespace shader                                    \
{

#define END_JLIB_GRAPHICS_SHADER_NAMESPACE      \
}                                                   \
END_JLIB_GRAPHICS_NAMESPACE

BEGIN_JLIB_GRAPHICS_SHADER_NAMESPACE

class surfaceShader;

END_JLIB_GRAPHICS_SHADER_NAMESPACE

#endif
