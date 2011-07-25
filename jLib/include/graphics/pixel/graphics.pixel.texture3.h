#ifndef JLIB_GRAPHICS_PIXEL_TEXTURE3_H
#define JLIB_GRAPHICS_PIXEL_TEXTURE3_H

#include "graphics/pixel/graphics.pixel.types.h"
#include "graphics/pixel/graphics.pixel.image.h"

BEGIN_JLIB_GRAPHICS_PIXEL_NAMESPACE

class JLIB_EXPORT texture3
    {
public:
    //! The openGL texture unit to load into, should fit into openGL limit
    unsigned char texUnit;
    unsigned int img;

    //! Create a blank texture
    texture3();
    //! Create a texture from an image class
    texture3( image &c );
    //! Create a texture from an image class
    //texture3( old::image &c );
    //! Load a texture from a file
    texture3( file::ioFile _f );
    //! Load a texture from a string containing a filename
    texture3( string _f );
    //! Destroy the texture
    ~texture3();
    //! Load the texture into openGL
    void load( );
    //! Load an image into the texture
    void loadImage( image &c );
    //void loadImage( old::image &c );
private:
    file::filename name;
    unsigned int width, height;
    };

END_JLIB_GRAPHICS_PIXEL_NAMESPACE

#endif
