#ifndef JLIB_GRAPHICS_PIXEL_TEXTURE4_H
#define JLIB_GRAPHICS_PIXEL_TEXTURE4_H

#include "graphics/pixel/graphics.pixel.types.h"
#include "graphics/pixel/graphics.pixel.image.h"

BEGIN_JLIB_GRAPHICS_PIXEL_NAMESPACE

class JLIB_EXPORT texture4
    {
public:
    //! The openGL texture unit to load into, should fit into openGL limit
    unsigned char texUnit;
    unsigned int img;

    //! Create a blank texture
    texture4();
    //! Create a texture from an image class
    texture4( image &c );
    //! Create a texture from an image class
    //texture3( old::image &c );
    //! Load a texture from a file
    texture4( file::ioFile _f );
    //! Load a texture from a string containing a filename
    texture4( string _f );
    //! Destroy the texture
    ~texture4();
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
