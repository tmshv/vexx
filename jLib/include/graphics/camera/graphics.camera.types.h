#ifndef JLIB_GRAPHICS_CAMERA_TYPES_H
#define JLIB_GRAPHICS_CAMERA_TYPES_H

#include "graphics/graphics.types.h"

#define BEGIN_JLIB_GRAPHICS_CAMERA_NAMESPACE        \
BEGIN_JLIB_GRAPHICS_NAMESPACE                       \
namespace camera                                    \
{

#define END_JLIB_GRAPHICS_CAMERA_NAMESPACE          \
}                                                   \
END_JLIB_GRAPHICS_NAMESPACE

BEGIN_JLIB_GRAPHICS_CAMERA_NAMESPACE

class base;
class perspective;
class orthographic;

END_JLIB_GRAPHICS_CAMERA_NAMESPACE

#endif
