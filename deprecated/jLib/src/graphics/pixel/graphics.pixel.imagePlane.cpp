#include "graphics/pixel/graphics.pixel.imagePlane.h"
#include "jLib.glHeader.h"

BEGIN_JLIB_GRAPHICS_PIXEL_NAMESPACE

imagePlane::imagePlane( entity::entityG *p, image cI, JFLOAT width ) : triangleCloud( p ), currentImage( 0, 0 ), _currentTexture( 0 ), _facing( FALSE )
    {
    setWidth( width );
    setImage( cI );
    setDrawable( );
    alphaBlend = TRUE;
    }

void imagePlane::setFacingCamera( bool _f )
    {
    _facing = _f;
    }

void imagePlane::setParalellCamera( bool _f )
    {
    _paralell = _f;
    }

imagePlane::~imagePlane( )
    {
    if( _currentTexture != 0 )
        {
        delete _currentTexture;
        }
    }

void imagePlane::setWidth( JFLOAT width )
    {
    _width = width;
    }

void imagePlane::setImage( image cI )
    {
    if( _currentTexture != 0 )
        {
        delete _currentTexture;
        _currentTexture = 0;
        }
    currentImage = cI;

    triangleCloud::clear();
    if( currentImage.isValid() )
        {
        float width = _width/2;
        float height =  (float)_width/currentImage.ratio() / 2;

        math::triple n;

        triangleCloud::addQuad( triangleCloud::addPoint( math::triple( width, height, 0 ), n, math::vector2( 1, 0 ), jLib::math::colour4::WHITE ),
                                triangleCloud::addPoint( math::triple( width, -height, 0 ), n, math::vector2( 1, 1 ), jLib::math::colour4::WHITE ),
                                triangleCloud::addPoint( math::triple( -width, -height, 0 ), n, math::vector2( 0, 1 ), jLib::math::colour4::WHITE ),
                                triangleCloud::addPoint( math::triple( -width, height, 0 ), n, math::vector2( 0, 0 ), jLib::math::colour4::WHITE ) );
        }
    }

void imagePlane::draw( )
    {
    jRenderDebug( "Draw ImagePlane" );
    if( _facing )
        {
        entityD::faceCamera();
        }
    if( _paralell )
        {
        entityD::parallelCamera();
        }
    if( _currentTexture == 0 && currentImage.isValid() )
        {
        jRenderDebug( "Allocate new texture ["<<currentImage.width()<<", "<<currentImage.height()<<"]" );
        _currentTexture = new texture( currentImage, texture::RGBA8 );
        }
    if( _currentTexture != 0 )
        {
        jRenderDebug( "Enable Texturing and bind texture" );
        glEnable( GL_TEXTURE_2D ) GLE;
        _currentTexture->bind();
        //glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE ) GLE;
        }

    jRenderDebug( "Draw quad" );
    triangleCloud::draw();
    if( _currentTexture != 0 )
        {
        _currentTexture->unbind();
        jRenderDebug( "Disable Texturing" );
        glDisable( GL_TEXTURE_2D ) GLE;
        }
    }

END_JLIB_GRAPHICS_PIXEL_NAMESPACE
