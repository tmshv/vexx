#include "graphics/camera/graphics.camera.base.h"
#include "jLib.glHeader.h"

BEGIN_JLIB_GRAPHICS_CAMERA_NAMESPACE

base::base( render::scene *_e ) : clipNear( 0.2 ), clipFar( 1000 ), engine( _e )
    {
    setChanged();
    }

base::~base( )
    {
    ;
    }

void base::setChanged( bool able )
    {
    _changed = able;
    }

bool base::changed( )
    {
    return _changed;
    }

math::matrix4x4 &base::projection()
    {
    return _projection;
    }

math::matrix4x4 &base::view()
    {
    return _view;
    }

math::matrix4x4 &base::inverseView()
    {
    return _inverseView;
    }

void base::update( )
    {
    if( _changed )
        {
        updateProjection();

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        math::triple oldN=N;
        N = facing();
        if( N == 0 )
            {
            N = oldN;
            }

        U=math::triple( up.normalise() ).crossProduct( N ).normalise();
        V=N.crossProduct( U ).normalise();

    //#warning need to fix the aim matrix thing bit.

        _view = math::matrix4x4::IDENTITY;

        _view.setTranslation( math::triple( -U.dotProduct( translate ), -V.dotProduct( translate ), -N.dotProduct( translate ) ) );

        _view.setXAxis( math::triple( U.x(), V.x(), N.x() ) );
        _view.setYAxis( math::triple( U.y(), V.y(), N.y() ) );
        _view.setZAxis( math::triple( U.z(), V.z(), N.z() ) );

        //mat.setTranslation( math::triple( 0, 0, 0 );


        //mat.multiplyCurrent();
        /*math::matrix4x4 mat2( U.x(), V.x(), N.x(), 0,
            U.y(), V.y(), N.y(), 0,
            U.z(), V.z(), N.z(), 0,
            -U.dotProduct( translate ), -V.dotProduct( translate ), -N.dotProduct( translate ), 1 );

        jDebugNeat<<( mat == mat2 )<<endl;*/

        onCameraChange();
        _changed = FALSE;
        }
    set();
    }

void base::set()
    {
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    _projection.multiplyCurrent();
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    _view.multiplyCurrent();
    _inverseView = _view.inverse();
    }

END_JLIB_GRAPHICS_CAMERA_NAMESPACE
