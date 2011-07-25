#ifndef JLIB_GRAPHICS_VIEWPORT_FRAMEBUFFER_H
#define JLIB_GRAPHICS_VIEWPORT_FRAMEBUFFER_H

#include "graphics/viewport/graphics.viewport.types.h"
#include "graphics/viewport/graphics.viewport.renderTarget.h"

BEGIN_JLIB_GRAPHICS_VIEWPORT_NAMESPACE

class JLIB_EXPORT frameBuffer : renderTarget
    {
public:
    frameBuffer();
    enum depthFormat { DEPTH_8BIT_INT, DEPTH_16BIT_FLOAT };
    enum colourFormat { COLOUR_32BIT_INT, COLOUR_64BIT_FLOAT };
    virtual void start();
    virtual void finish();
    virtual void setSize( unsigned int , unsigned int );
    void setDepthFormat( depthFormat );
    void setColourFormat( colourFormat );
    int depthTextureID();
    int colourTextureID();
    void setDepthTextureUnit( unsigned int );
    void setColourTextureUnit( unsigned int );
    void bindTextures();
    };

END_JLIB_GRAPHICS_VIEWPORT_NAMESPACE

#endif
