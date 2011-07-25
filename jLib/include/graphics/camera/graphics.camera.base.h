#ifndef JLIB_GRAPHICS_CAMERA_BASE_H
#define JLIB_GRAPHICS_CAMERA_BASE_H

#include "graphics/camera/graphics.camera.types.h"
#include "graphics/shape/graphics.shape.line.h"
#include "jLib.informer.h"

BEGIN_JLIB_GRAPHICS_CAMERA_NAMESPACE

jLibInformerObserverPairNoArguments( JLIB_EXPORT, cameraObserver, onCameraChange, cameraInformer )

//! A virtual class for describing cameras
class JLIB_EXPORT base : public cameraInformer
    {
public:
    typedef enum
        {
        NONE,
        ROTATE,
        TRANSLATE,
        ZOOM,
        ZOOMINC
        } motionType;
    //! Hold the REAL directions relative to the camera
    math::triple U, N, V;
    //! The position of the camera
    math::triple translate;
    //! The direction the camera faces
    math::triple direction;
    //! The approximate up direction
    math::triple up;
    //! The near clipping plane distance from the view center
    JFLOAT clipNear;
    //! The far clipping plane distance from the view center
    JFLOAT clipFar;

    //! Initiates the camera
    base( render::scene *_e );
    //! Destructor
    virtual ~base( );

    math::matrix4x4 &projection();

    math::matrix4x4 &view();
    math::matrix4x4 &inverseView();

    void setChanged( bool=true );
    bool changed( );
    //! Returns the vector facing directly from the camera
    virtual math::triple facing() { return math::triple( translate - direction ).normalise(); }
    //! Loads the camera into the openGL matricies
    virtual void update();
    //! Reset the GL projection matrix, called from update.
    virtual void updateProjection() = 0;
    //! Converts from screen to world space
    virtual shape::line screenToVector( int x, int y ) = 0;
    //! Applies high level motion types that you might expect in a higer level 3D program
    virtual void motion( motionType type, float x, float y ) = 0;
    //! to be called to set the openGL matricies say when the camera is swapped for another
    void set();
protected:
    math::matrix4x4 _projection;
    math::matrix4x4 _view;
    math::matrix4x4 _inverseView;
    //! The engine that partners the camera
    render::scene *engine;
    //! Whether the camera needs updating
    bool _changed;
    };

END_JLIB_GRAPHICS_CAMERA_NAMESPACE

#endif
