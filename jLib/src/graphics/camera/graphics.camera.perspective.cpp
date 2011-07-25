#include "graphics/camera/graphics.camera.perspective.h"
#include "graphics/render/graphics.render.scene.h"
#include "jLib.glHeader.h"

BEGIN_JLIB_GRAPHICS_CAMERA_NAMESPACE

//Credit for this function comes from http://www.opengl.org/ and the listing for the functions glFrustrum and glPerspective
math::matrix4x4 perspectiveMatrix( JFLOAT fovy, JFLOAT aspect, JFLOAT nearClip, JFLOAT farClip )
    {
    math::matrix4x4 ret = math::matrix4x4::IDENTITY;
    JFLOAT tempF = 1 / ( tan((fovy*DEGTORAD)/2) );
    JFLOAT nearSubFar = nearClip - farClip;

    ret.RM(0,0) = tempF / aspect;
    ret.RM(1,1) = tempF;
    ret.RM(2,2) = ( ( farClip + nearClip) / nearSubFar );
    ret.RM(3,2) = -1;
    ret.RM(2,3) = ( ( 2 * farClip * nearClip ) / nearSubFar );
    ret.RM(3,3) = 0;

    return ret;
    }

perspective::perspective( render::scene *_e, math::triple _t, math::triple _d, math::triple _u ) : base( _e )
    {
    translate = _t;
    direction = _d;
    up = _u;
    fov=55.0;
    }

void perspective::updateProjection( )
    {
    //glMatrixMode( GL_PROJECTION );
    //glLoadIdentity();
    _projection = perspectiveMatrix( fov, engine->ratio(), clipNear, clipFar );
    }

void perspective::motion( motionType type, float x, float y )
    {
    static math::triple oldTranslate, oldDirection;
    static float xBegin=-1, yBegin=-1;
    float xReal, yReal;
    if( yBegin == -1 && xBegin == -1 && ( type == ROTATE || type == TRANSLATE || type == ZOOM ) )
        {
        //cout<<"RESET"<<endl;
        oldTranslate = translate;
        oldDirection = direction;
        xBegin=x;
        yBegin=y;
        }

    xReal = x - xBegin;
    yReal = y - yBegin;

    if( type == ROTATE )
        {
        //cout<<yReal<<" "<<xReal<<endl;
        if( !math::fcmp(xReal,0) || !math::fcmp(yReal,0) )
            {
            math::polar polar( oldTranslate - direction );

            polar.phi() += ROT_X_FACTOR*xReal;

            polar.theta() -= ROT_Y_FACTOR*yReal;

            translate = direction + polar.getPoint();
            _changed = TRUE;
            }
        }
    else if( type == TRANSLATE )
        {
        math::triple temp = U*(xReal*TRANS_X_FACTOR) + V*(yReal*TRANS_Y_FACTOR);
        direction = oldDirection + temp;
        translate = oldTranslate + temp;
        _changed = TRUE;
        }
    else if( type == ZOOM )
        {
        math::triple temp = N*(yReal*ZOOM_FACTOR);
        translate = oldTranslate + temp;
        direction = oldDirection + temp;
        _changed = TRUE;
        }
    else if( type == ZOOMINC )
        {
        math::triple temp = N*(y*ZOOMINC_FACTOR);
        if( y > 0 && temp.length() < (0.5+(y*ZOOM_FACTOR)) )
            {
            direction = oldDirection + temp;
            }
        translate = oldTranslate + temp;
        _changed = TRUE;
        oldTranslate = translate;
        oldDirection = direction;
        }
    else
        {
        xBegin=-1;
        yBegin=-1;
        }
    }

shape::line perspective::screenToVector( int x, int y )
    {
    //cout<<((2*((float)x/(float)engine->width))-1)<<" "<<((2*((float)y/(float)engine->height))-1)<<endl;
    float fovMult = tan((fov*DEGTORAD)/2);
    math::triple look = ( direction - translate ).normalise();
    math::triple across = look.crossProduct( up ).scale( engine->ratio() ) * fovMult * ((2*((float)x/(float)engine->width()))-1);
    math::triple realUp = across.crossProduct( look ).normalise() * fovMult * ((2*((float)y/(float)engine->height()))-1);

    if( x > (int)(engine->width()/2) )
        {
        realUp *= -1;
        }

    return shape::line( translate, ( direction - translate ).normalise() + across + realUp );
    }

END_JLIB_GRAPHICS_CAMERA_NAMESPACE
