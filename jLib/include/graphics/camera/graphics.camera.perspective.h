#ifndef JLIB_GRAPHICS_CAMERA_PERSPECTIVE_H
#define JLIB_GRAPHICS_CAMERA_PERSPECTIVE_H

#include "graphics/camera/graphics.camera.types.h"
#include "graphics/camera/graphics.camera.base.h"

#define POINT 0
#define DIRECTION 1

#define ZOOMINC_FACTOR 2
#define ZOOM_FACTOR -50
#define TRANS_Y_FACTOR 20
#define TRANS_X_FACTOR -20
#define ROT_X_FACTOR 5
#define ROT_Y_FACTOR 5

BEGIN_JLIB_GRAPHICS_CAMERA_NAMESPACE

//! get a perspective projection matrix
math::matrix4x4 perspectiveMatrix( JFLOAT fovy, JFLOAT aspect, JFLOAT near, JFLOAT far );

class JLIB_EXPORT perspective : public base
    {
public:
    //! The Field of View used in perspective calculations
    float fov;

    //! create a camera with reference to world, translation, direction and up
    perspective( render::scene *_e, math::triple _t, math::triple _d, math::triple _u=math::triple( 0, 1, 0 ) );
    //! create a camera with reference to world, translation and direction
    //cameraPerspective( view *_e, math::point4 _t, math::triple _d );
    //! Update the camera in openGL, should be called after changing the camera
    void updateProjection( );
    //! Create motion you might expect from a 3D package
    void motion( motionType type, float x, float y );
    //! transform a vector from screen to global world space
    shape::line screenToVector( int x, int y );
    };

END_JLIB_GRAPHICS_CAMERA_NAMESPACE

#endif
