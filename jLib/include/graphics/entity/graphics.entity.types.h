#ifndef JLIB_GRAPHICS_ENTITY_TYPES_H
#define JLIB_GRAPHICS_ENTITY_TYPES_H

#include "graphics/graphics.types.h"

#define BEGIN_JLIB_GRAPHICS_ENTITY_NAMESPACE        \
BEGIN_JLIB_GRAPHICS_NAMESPACE                       \
namespace entity                                    \
{

#define END_JLIB_GRAPHICS_ENTITY_NAMESPACE          \
}                                                   \
END_JLIB_GRAPHICS_NAMESPACE

BEGIN_JLIB_GRAPHICS_ENTITY_NAMESPACE

class bBox;
class entityB;
class entityG;
class entityD;

END_JLIB_GRAPHICS_ENTITY_NAMESPACE

#endif
