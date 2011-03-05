#ifndef JLIB_GRAPHICS_PIXEL_TEXTURE_H
#define JLIB_GRAPHICS_PIXEL_TEXTURE_H

#include "graphics/pixel/graphics.pixel.types.h"
#include "graphics/pixel/graphics.pixel.image.h"

class QImage;

BEGIN_JLIB_GRAPHICS_PIXEL_NAMESPACE


class JLIB_EXPORT texture
    {
public:
    //! texture wrap mode
    enum wrapMode { wrapClamp, wrapRepeat };
    //! formats for images
    enum textureFormat { RGBA8, RGBA16F, RGBA32F, RGB8, RGB16F, RGB32F, DEPTH16, DEPTH32F };
    //! Construct with width and height
    texture( unsigned int width=1, unsigned int height=1, textureFormat=RGB8, JFLOAT *data=NULL );
    //! Construct from image
    //! \warning constructing with a format which has no alpha is not supported... oops.
    texture( const image &, textureFormat=RGB8 );
    //! Construct from a QImage
    texture( QImage );
    //! Destroy the texture
    ~texture();
    //! Find if a texture is valid
    bool isValid( ) const;
    //! Find the image width
    unsigned int width() const;
    //! Find the image width
    unsigned int height() const;
    //! The openGL texture id
    int textureID() const;
    //! The current openGL texture unit
    int textureUnit() const;
    //! The current openGL texture unit
    void setTextureUnit( int );
    //! Set the texture wrap mode
    void setTextureWrap( wrapMode=wrapClamp );
    //! Load the texture into openGL
    void bind( ) const;
    //! Un load the texture into openGL
    void unbind( ) const;
    //! Set the size of the texture, may require creation of a new internal texture
    void setSize( unsigned int, unsigned int );

private:
    int getInternalFormat( textureFormat );
    int getFormat( textureFormat );
    //! The internal format of the image
    textureFormat _format;
    //! The openGL texture unit to load into, should fit into openGL limit
    int _texUnit;
    //! Current allocated openGL texture id
    unsigned int _texID;
    //! Width of image
    unsigned int _width;
    //! Height of image
    unsigned int _height;
    };

END_JLIB_GRAPHICS_PIXEL_NAMESPACE

#endif
