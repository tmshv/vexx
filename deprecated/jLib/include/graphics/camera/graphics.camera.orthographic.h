#ifndef JLIB_GRAPHICS_CAMERA_ORTHOGRAPHIC_H
#define JLIB_GRAPHICS_CAMERA_ORTHOGRAPHIC_H

#include "graphics/camera/graphics.camera.types.h"
#include "graphics/camera/graphics.camera.base.h"

#define ORTHO_ZOOM_FACTOR 25
#define ORTHO_TRANS_Y_FACTOR 20
#define ORTHO_TRANS_X_FACTOR -20
#define ORTHO_ROT_X_FACTOR 5
#define ORTHO_ROT_Y_FACTOR 5

BEGIN_JLIB_GRAPHICS_CAMERA_NAMESPACE

//! Get an orthogonal projection matrix
math::matrix4x4 orthographicMatrix( float right, float left, float top, float bottom, float clipNear, float clipFar );

class JLIB_EXPORT orthographic : public base
    {
public:
    //! Initiate the camera with a reference to the world only, creating a one to one camera
    orthographic( render::scene &_e );
    //! Create a camera with reference to the world, and 4 clipping planes
    orthographic( render::scene &_e, float _b, float _l, float _t, float _r );
    //! Set the camera in openGL should be called after changing and clipping planes
    void updateProjection( );
    //! Set the width of the projection in world space
    void width( float w );
    //! Set the height of the projection in world space
    void height( float h );
    //! Get the width of the projection in world space
    float width( );
    //! Get the height of the projection in world space
    float height( );
    //! Transform a vector from screen space to global world space
    shape::line screenToVector( int x, int y );
    //! Create motion you might expect in a 3D package
    void motion( motionType type, float x, float y );
    //! Set the camera to a one to one coordinate system, using 0,0 as the center
    void setOneToOne( );
    //! Set the camera to a one to one coordinate system, using the specified center
    void setOneToOne( int _cX, int _cY );
private:
    union
        {
        struct { JFLOAT r, l, t, b; };
        JFLOAT cmp[4];
        };
    };

END_JLIB_GRAPHICS_CAMERA_NAMESPACE

#endif
