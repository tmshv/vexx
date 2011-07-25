#include "graphics/render/graphics.render.frameBuffer.h"
#include "graphics/pixel/graphics.pixel.image.h"
#include "jLib.glHeader.h"

BEGIN_JLIB_GRAPHICS_RENDER_NAMESPACE

frameBuffer::frameBuffer( int type, colourFormat col, depthFormat dep ) : _colour( 0 ), _depth( 0 ), _buffer( 0 )
    {
    jAssert( glewIsSupported("GL_VERSION_2_0") && "Ensure you have tried to create the shader while the gl context is active." );

    _type = type;
    _cFormat = col;
    _dFormat = dep;

    glGenFramebuffersEXT( 1, &_buffer ) GLE;

     if( ( _type & COLOUR ) != FALSE )
        {
        _colour = new pixel::texture( 1, 1, (pixel::texture::textureFormat)col );
        }

    if( ( _type & DEPTH ) != FALSE )
        {
        _depth = new pixel::texture( 1, 1, (pixel::texture::textureFormat)dep );
        }
    }

void frameBuffer::clear()
    {
    if( _colour )
        {
        delete _colour;
        }
    if( _depth )
        {
        delete _depth;
        }
    if( _buffer )
        {
        glDeleteFramebuffersEXT( 1, &_buffer ) GLE;
        }
    }

frameBuffer::~frameBuffer()
    {
    clear();
    }

void frameBuffer::start()
    {
    jAssert( isValid() );
    glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, _buffer );
    }

void frameBuffer::finish()
    {
    glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
    }

void frameBuffer::setSize( unsigned int x, unsigned int y )
    {
    jRenderDebug( "Resize fBuffer to "<<x<<" "<<y );
    glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, _buffer );

    if( ( _type & COLOUR ) != FALSE )
        {
        _colour->setSize( x, y );
        glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, _colour->textureID(), 0 );
        }

    if( ( _type & DEPTH ) != FALSE )
        {
        _depth->setSize( x, y );
        glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, _depth->textureID(), 0 );
        }

    jAssert( isValid() );

    glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
    }

pixel::texture &frameBuffer::texture( mode type )
    {
    if( type == COLOUR )
        {
        return *_colour;
        }
    else if( type == DEPTH )
        {
        return *_depth;
        }
    jFail( "Bad input" );
    return *_colour;
    }

bool frameBuffer::has( mode type )
    {
    if( type == COLOUR )
        {
        return _colour != 0;
        }
    else if( type == DEPTH )
        {
        return _depth != 0;
        }
    return 0;
    }

void frameBuffer::bindTextures()
    {
    if( _colour )
        {
        _colour->bind();
        }
    if( _depth )
        {
        _depth->bind();
        }
    }

void frameBuffer::unbindTextures()
    {
    if( _colour )
        {
        _colour->unbind();
        }
    if( _depth )
        {
        _depth->unbind();
        }
    }

bool frameBuffer::isValid()
    {
    glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, _buffer );
    int status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);

    /*if( status == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT )
        {
        jFail( "Framebuffer Incomplete attachment" );
        }
    else if( status == GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT )
        {
        jFail( "Framebuffer Incomplete dimensions" );
        }
    else if( status == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT )
        {
        jFail( "Framebuffer Incomplete missing attachment" );
        }
    else if( status == GL_FRAMEBUFFER_UNSUPPORTED_EXT )
        {
        jFail( "Framebuffer unsupported attachment" );
        }*/

    glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );

    return status == GL_FRAMEBUFFER_COMPLETE_EXT;
    }

int frameBuffer::textureID( mode type )
    {
    if( type == COLOUR )
        {
        return _colour->textureID();
        }
    else if( type == DEPTH )
        {
        return _depth->textureID();
        }
    return 0;
    }

jLib::math::colour4 frameBuffer::colourAt( int x, int y )
    {
    jLib::math::colour4 ret;
    start();

    glReadPixels(x, y, 1, 1, GL_RGBA, GL_FLOAT, &ret[0] ) GLE;

    finish();
    return ret;
    }

END_JLIB_GRAPHICS_RENDER_NAMESPACE
