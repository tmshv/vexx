#include "graphics/pixel/graphics.pixel.texture.h"
#include "jLib.glHeader.h"
#include "QImage"

BEGIN_JLIB_GRAPHICS_PIXEL_NAMESPACE

#define clean( tex ) if( tex->isValid() ) { glDeleteTextures( 1, &this->_texID ) GLE; }

texture::texture( unsigned int w, unsigned int h, textureFormat f, JFLOAT *d ) : _texUnit( 0 ), _texID( 0 )
    {
    _width = w;
    _height = h;
    _format = f;

    glGenTextures( 1, &_texID ) GLE;
    glBindTexture( GL_TEXTURE_2D, _texID ) GLE;

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST ) GLE;
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST ) GLE;
    setTextureWrap();

    glTexImage2D( GL_TEXTURE_2D, 0, getInternalFormat( f ), _width, _height, 0, getFormat( f ), GL_UNSIGNED_BYTE, (const GLvoid *)d ) GLE;

    glBindTexture( GL_TEXTURE_2D, 0 ) GLE;
    }

texture::texture( const image &in, textureFormat format ) : _texUnit( 0 ), _texID( 0 )
    {
    jAssert( in.isValid() );
    jRenderDebug( "Construct texture from image, ["<<in.width()<<","<<in.height()<<"]" );

    _height = in.height();
    _width = in.width();
    _format = format;

    glGenTextures( 1, &_texID ) GLE;
    glBindTexture( GL_TEXTURE_2D, _texID ) GLE;

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST ) GLE;
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST ) GLE;
    setTextureWrap();

    glTexImage2D( GL_TEXTURE_2D, 0, getInternalFormat( _format ), _width, _height, 0, getFormat( _format ), GL_FLOAT, (const GLvoid *)in.at( 0, 0 ) ) GLE;

    glBindTexture( GL_TEXTURE_2D, 0 ) GLE;
    }

texture::texture( QImage in ) : _texUnit( 0 ), _texID( 0 )
    {
    jRenderDebug( "Construct texture from image, ["<<in.width()<<","<<in.height()<<"]" );

    in.convertToFormat( QImage::Format_ARGB32_Premultiplied );

    _height = in.height();
    _width = in.width();
    _format = RGBA8;

    glGenTextures( 1, &_texID ) GLE;
    glBindTexture( GL_TEXTURE_2D, _texID ) GLE;

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST ) GLE;
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST ) GLE;
    setTextureWrap();

    glTexImage2D( GL_TEXTURE_2D, 0, getInternalFormat( _format ), _width, _height, 0, GL_BGRA, GL_UNSIGNED_BYTE, in.bits() ) GLE;

    glBindTexture( GL_TEXTURE_2D, 0 ) GLE;
    }

texture::~texture()
    {
    clean( this );
    }

bool texture::isValid( ) const
    {
    return glIsTexture( _texID );
    }

unsigned int texture::width() const
    {
    return _width;
    }

unsigned int texture::height() const
    {
    return _height;
    }

int texture::textureID() const
    {
    return _texID;
    }

int texture::textureUnit() const
    {
    return _texUnit;
    }

void texture::setTextureUnit( int unit )
    {
    _texUnit = unit;
    }

void texture::setTextureWrap( wrapMode mode )
    {
    if( mode == wrapClamp )
        {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP) GLE;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP) GLE;
        }
    else if( mode == wrapRepeat )
        {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT) GLE;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT) GLE;
        }
    else
        {
        jFail( "Incorrect texture wrap mode" );
        }
    }

void texture::bind( ) const
    {
    jRenderDebug( "Bind Texture "<<_texID<<" to unit "<<_texUnit );
    glActiveTexture( GL_TEXTURE0+_texUnit ) GLE;
    glBindTexture( GL_TEXTURE_2D, _texID ) GLE;
    }

void texture::unbind( ) const
    {
    jRenderDebug( "Unbind Texture "<<_texID<<" from unit "<<_texUnit );
    glActiveTexture( GL_TEXTURE0+_texUnit ) GLE;
    glBindTexture( GL_TEXTURE_2D, 0 ) GLE;
    }

int texture::getFormat( textureFormat in )
    {
    switch( in )
        {
        case RGBA8:
            return GL_RGBA;
        case RGBA16F:
            return GL_RGBA;
        case RGBA32F:
            return GL_RGBA;
        case RGB8:
            return GL_RGB;
        case RGB16F:
            return GL_RGB;
        case RGB32F:
            return GL_RGB;
        case DEPTH16:
            return GL_DEPTH_COMPONENT;
        case DEPTH32F:
            return GL_DEPTH_COMPONENT;
        }
    return GL_RGBA8;
    }

int texture::getInternalFormat( textureFormat in )
    {
    switch( in )
        {
        case RGBA8:
            return GL_RGBA8;
        case RGBA16F:
            return GL_RGBA16F_ARB;
        case RGBA32F:
            return GL_RGBA32F_ARB;
        case RGB8:
            return GL_RGB8;
        case RGB16F:
            return GL_RGBA16F_ARB;
        case RGB32F:
            return GL_RGB32F_ARB;
        case DEPTH16:
            return GL_DEPTH_COMPONENT16;
        case DEPTH32F:
            return GL_DEPTH_COMPONENT32F_NV;
        }
    return GL_RGBA8;
    }

void texture::setSize( unsigned int w, unsigned int h )
    {
    clean( this );

    _width = w;
    _height = h;

    glGenTextures( 1, &_texID ) GLE;
    glBindTexture( GL_TEXTURE_2D, _texID ) GLE;

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST ) GLE;
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST ) GLE;
    setTextureWrap();


    glTexImage2D( GL_TEXTURE_2D, 0, getInternalFormat( _format ), _width, _height, 0, getFormat( _format ), GL_UNSIGNED_BYTE, NULL ) GLE;

    glBindTexture( GL_TEXTURE_2D, 0 ) GLE;
    }


END_JLIB_GRAPHICS_PIXEL_NAMESPACE
