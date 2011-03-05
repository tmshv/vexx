#ifndef JLIB_GRAPHICS_VIEWPORT_FRAMEBUFFER_H
#define JLIB_GRAPHICS_VIEWPORT_FRAMEBUFFER_H

#include "graphics/render/graphics.render.types.h"
#include "graphics/render/graphics.render.target.h"
#include "graphics/pixel/graphics.pixel.texture.h"

BEGIN_JLIB_GRAPHICS_RENDER_NAMESPACE

class JLIB_EXPORT frameBuffer : public render::target
    {
public:
    enum mode { COLOUR = 1, DEPTH = 2 };
    enum depthFormat { DEPTH_8BIT_INT=pixel::texture::DEPTH16, DEPTH_32BIT_FLOAT=pixel::texture::DEPTH32F };
    enum colourFormat { COLOUR_32BIT_INT=pixel::texture::RGBA8, COLOUR_64BIT_FLOAT=pixel::texture::RGBA16F, COLOUR_128BIT_FLOAT=pixel::texture::RGBA32F };
    frameBuffer( int=COLOUR|DEPTH, colourFormat=COLOUR_32BIT_INT, depthFormat=DEPTH_8BIT_INT );
    ~frameBuffer();

    jLib::math::colour4 colourAt( int, int );

    virtual void start();
    virtual void finish();
    virtual void setSize( unsigned int , unsigned int );
    pixel::texture &texture( mode );
    int textureID( mode );
    bool has( mode );
    void bindTextures();
    void unbindTextures();
    bool isValid();
    void clear();

private:
    pixel::texture *_colour;
    pixel::texture *_depth;
    colourFormat _cFormat;
    depthFormat _dFormat;
    unsigned int _buffer;
    int _type;
    };

END_JLIB_GRAPHICS_RENDER_NAMESPACE

#endif
