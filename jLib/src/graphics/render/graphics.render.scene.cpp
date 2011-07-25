#include "graphics/render/graphics.render.scene.h"
#include "jLib.glHeader.h"

BEGIN_JLIB_GRAPHICS_RENDER_NAMESPACE

scene::scene( math::colour4 col, unsigned int w, unsigned int h ) : _target( 0 ),
        _defaultScene( 0 ), _defaultCamera( this, math::triple( 10, 10, 10 ), 0 )
    {
    _bgCol = col;
    _width = w;
    _height = h;
    glViewport( 0, 0, _width, _height );
    addScene( &_defaultScene );
    addCamera( _defaultCamera );
    }

scene::~scene( )
    {
    }

void scene::addScene( entity::entityG *in )
    {
    jAssert( in );
    FOREACH( _passes, pass )
        {
        if( *pass == in )
            {
            return;
            }
        }
    _passes.push_back( in );
    setPassCamera( _passes.size() - 1, _defaultCamera );
    }

void scene::removeScene( entity::entityG *in )
    {
    FOREACH( _passes, pass )
        {
        if( *pass == in )
            {
            _passes.erase( pass );
            return;
            }
        }
    }

void scene::setPassCamera( unsigned int x, camera::base &cam )
    {
    jAssert( x < _passes.size() );
    _passes[x]->_currentCamera = &cam;
    }

void scene::setRenderTarget( render::target &in )
    {
    _target = &in;
    setSize( _width, _height );
    }

void scene::clearRenderTarget( )
    {
    _target = 0;
    }

void scene::removeScene( unsigned int x )
    {
    jAssert( x < _passes.size() );
    removeScene( _passes[x] );
    }

entity::entityG &scene::pass( unsigned int x )
    {
    jAssert( x < _passes.size() );
    return *_passes[x];
    }

entity::entityG *scene::passP( unsigned int x )
    {
    jAssert( x < _passes.size() );
    return _passes[x];
    }

camera::base &scene::defaultCamera()
    {
    return _defaultCamera;
    }

void scene::addCamera( camera::base &in )
    {
    FOREACH( _cameras, cam )
        {
        if( *cam == &in )
            {
            return;
            }
        }
    _cameras.push_back( &in );
    }

void scene::removeCamera( unsigned int x )
    {
    jAssert( x < _cameras.size() );
    removeCamera( *_cameras[x] );
    }

void scene::removeCamera( camera::base &in )
    {
    if( &in != &_defaultCamera )
        {
        FOREACH( _cameras, cam )
            {
            if( *cam == &in )
                {
                _cameras.erase( cam );
                }
            }
        }
    }

camera::base &scene::camera( unsigned int x )
    {
    jAssert( x < _cameras.size() );
    return *_cameras[x];
    }

math::colour4 scene::bgColour()
    {
    return _bgCol;
    }

/*unsigned int scene::width()
    {
    return _width;
    }

unsigned int scene::height()
    {
    return _height;
    }*/

JFLOAT scene::ratio()
    {
    return (float)_width / (float)_height;
    }

void scene::setSize( unsigned int w, unsigned int h )
    {
    jRenderDebug( "Resize scene to "<<w<<" "<<h );
    _width = w;
    _height = h;
    glViewport( 0, 0, _width, _height );
    FOREACH( _passes, pass )
        {
        (*pass)->camera()->setChanged();
        }
    if( _target )
        {
        _target->setSize( _width, _height );
        }
    }

void scene::setBgColour( math::colour4 in )
    {
    _bgCol = in;
    }

void scene::render()
    {
    if( _target )
        {
        jRenderDebug( "Start target "<<_target );
        _target->start();
        }

    glClearColor( _bgCol.r(), _bgCol.g(), _bgCol.b(), _bgCol.a() );

    jRenderDebug( "Clear Screen" );
    glClear( GL_COLOR_BUFFER_BIT );

    FOREACH( _passes, pass )
        {
        jRenderDebug( "Render Pass 1" );
        glClear( GL_DEPTH_BUFFER_BIT );
        (*pass)->camera()->update();
        (*pass)->draw();
        }

    if( _target )
        {
        jRenderDebug( "Finish target "<<_target );
        _target->finish();
        }
    }

END_JLIB_GRAPHICS_RENDER_NAMESPACE
