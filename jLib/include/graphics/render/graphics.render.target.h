#ifndef JLIB_GRAPHICS_VIEWPORT_TARGET_H
#define JLIB_GRAPHICS_VIEWPORT_TARGET_H

#include "graphics/render/graphics.render.types.h"

BEGIN_JLIB_GRAPHICS_RENDER_NAMESPACE

class JLIB_EXPORT target
    {
public:
    virtual ~target();
    virtual void start() = 0;
    virtual void finish() = 0;
    virtual void setSize( unsigned int , unsigned int ) = 0;
    };

END_JLIB_GRAPHICS_RENDER_NAMESPACE

#endif
