#ifndef JLIB_GRAPHICS_COLLADA_TYPES_H
#define JLIB_GRAPHICS_COLLADA_TYPES_H

#include "graphics/graphics.types.h"
#include "xml/xml.h"

#define BEGIN_JLIB_GRAPHICS_COLLADA_NAMESPACE           \
BEGIN_JLIB_GRAPHICS_NAMESPACE                           \
namespace collada                                       \
{

#define END_JLIB_GRAPHICS_COLLADA_NAMESPACE             \
}                                                       \
END_JLIB_GRAPHICS_NAMESPACE

BEGIN_JLIB_GRAPHICS_COLLADA_NAMESPACE

class base;
class file;

END_JLIB_GRAPHICS_COLLADA_NAMESPACE

#endif
