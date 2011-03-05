#include "graphics/camera/graphics.camera.orthographic.h"
#include "graphics/render/graphics.render.scene.h"
#include "jLib.glHeader.h"

BEGIN_JLIB_GRAPHICS_CAMERA_NAMESPACE

//Credit for this function comes from http://www.opengl.org/ and the listing for the functions glOrtho
math::matrix4x4 orthographicMatrix( float right, float left, float top, float bottom, float clipNear, float clipFar )
    {
    math::matrix4x4 ret = math::matrix4x4::IDENTITY;

    ret.CM(0,0) = 2 / ( right - left );
    ret.CM(1,1) = 2 / ( top - bottom );
    ret.CM(2,2) = -2 / ( clipFar - clipNear );
    ret.CM(0,3) = - ( right + left ) / ( right - left );
    ret.CM(1,3) = - ( top + bottom ) / ( top - bottom );
    ret.CM(2,3) = - ( clipFar + clipNear ) / ( clipFar - clipNear );
    ret.CM(3,3) = 1;

    return ret;
    }

void orthographic::width( float w )
    {
    w = fabs( w );
    r = w/2;
    l = -w/2;
    t = ((engine->ratio())*w)/2;
    b = (-(engine->ratio())*w)/2;
    }
void orthographic::height( float h )
    {
    h = fabs( h );
    t = h/2;
    b = -h/2;
    t = ((engine->ratio())*h)/2;
    b = (-(engine->ratio())*h)/2;
    }
float orthographic::width(  )
    {
    return r-l;
    }
float orthographic::height( )
    {
    return t-b;
    }

void orthographic::updateProjection()
    {
    //glMatrixMode( GL_PROJECTION );
    //glLoadIdentity();
    _projection = orthographicMatrix( r, l, t, b, clipNear, clipFar );
    }

void orthographic::motion( motionType type, float x, float y )
    {
    static math::triple oldTranslate, oldDirection;
    static float xBegin=-1, yBegin=-1;
    float xReal, yReal;
    if( yBegin == -1 && xBegin == -1 && ( type == ROTATE || type == TRANSLATE || type == ZOOM ) )
        {
        oldTranslate = translate;
        oldDirection = direction;
        xBegin=x;
        yBegin=y;
        }

    xReal = x - xBegin;
    yReal = y - yBegin;

    if( type == ROTATE )
        {
        if( !math::fcmp(xReal,0) || !math::fcmp(yReal,0) )
            {
            math::polar polar(oldTranslate - direction);

            polar.phi() += ORTHO_ROT_Y_FACTOR*yReal;
            polar.theta() += ORTHO_ROT_X_FACTOR*xReal;
            translate = direction + polar.getPoint();
            _changed = TRUE;
            }
        }
    else if( type == TRANSLATE )
        {
        math::triple temp = U*(xReal*ORTHO_TRANS_X_FACTOR) + V*(yReal*ORTHO_TRANS_Y_FACTOR);
        direction = oldDirection + temp;
        translate = oldTranslate + temp;
        _changed = TRUE;
        }
    else if( type == ZOOM )
        {
        jWarning<<"Ortho Can't Zoom";
        }
    else if( type == ZOOMINC )
        {
        jWarning<<"Ortho Can't ZoomInc";
        }
    else
        {
        xBegin=-1;
        yBegin=-1;
        }
    }

shape::line orthographic::screenToVector( int x, int y )
    {
    float xPercent = ( (float)x/(float)engine->width() ) - 0.5;
    float yPercent = 0.5 - ( (float)y/(float)engine->height() );

    return shape::line( translate + N + U.scale( xPercent * ( width() ) ) + V.scale( yPercent * ( height() ) ), N );
    }
void orthographic::setOneToOne( )
    {
    setOneToOne( 0, 0 );
    }

void orthographic::setOneToOne( int _cX, int _cY )
    {
    b =_cX - (engine->height()/2);
    l =_cY - (engine->width()/2);
    t =_cY + (engine->height()/2);
    r =_cX + (engine->width()/2);

    _changed = TRUE;
    }

orthographic::orthographic( render::scene &_e ) : base( &_e )
    {
    setOneToOne( );
    translate = math::triple(0,0,5);
    direction = math::triple(0,0,1);
    up = math::triple(0,1,0);
    }

orthographic::orthographic( render::scene &_e, float _b, float _l, float _t, float _r ) : base( &_e )
    {
    b = _b;
    l = _l;
    t = _t;
    r = _r;

    translate = math::triple(0,0,0);
    direction = math::triple(0,0,-1);
    up = math::triple(0,1,0);
    }

END_JLIB_GRAPHICS_CAMERA_NAMESPACE
