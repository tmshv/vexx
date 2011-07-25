#ifndef XCAMERA_H
#define XCAMERA_H

#include "X3DGlobal.h"
#include "XObject"
#include "XProperty"
#include "XVector3D"
#include "XTransform.h"

class XScene;

class XCamera : public XObject
    {
    X_OBJECT( XCamera, XObject, 5 )
public:
    XROProperty( XVector3D, position );
    XROProperty( XVector3D, aimPosition );
    XROProperty( XVector3D, upDirection );

    XROProperty( QSize, viewportSize );

    XROProperty( xReal, aspectRatio );

    XROProperty( XTransform, projectionTransform );
    XROProperty( XTransform, viewTransform );

    XProperty( XScene *, currentScene, setCurrentScene );

public:
    XCamera( XVector3D position, XVector3D aimPosition=XVector3D( 0, 0, 0 ), XVector3D up=XVector3D( 0, 1, 0 ) );

    void setUpDirection( XVector3D );
    void setPosition( XVector3D );
    void setAimDirection( XVector3D );
    void setAimPosition( XVector3D );

    void setViewportSize( QSize );

    XVector3D aimDirection() const;
    XVector3D vertical() const;
    XVector3D horizontal() const;

    void pan( xReal x, xReal y );
    void track( xReal x, xReal y, xReal z );

    X_SIGNAL( viewChanged );
    X_SIGNAL( projectionChanged );

protected:
    void update();

    void setProjectionTransform( XTransform );
    virtual void aspectRatioChanged();

private:
    void setTransform();
    };

class XPerspectiveCamera : public XCamera
    {
    X_OBJECT( XCamera, XObject, 6 )
public:
    XROProperty( xReal, viewAngle )
    XROProperty( xReal, nearClipPlane )
    XROProperty( xReal, farClipPlane )

public:
    XPerspectiveCamera( xReal angle, XVector3D position, XVector3D aimPosition=XVector3D( 0, 0, 0 ), XVector3D up=XVector3D( 0, 1, 0 ), xReal near=1.0, xReal far=1000.0 );

    void setViewAngle( xReal );
    void setNearClipPlane( xReal );
    void setFarClipPlane( xReal );

private:
    void aspectRatioChanged();
    void updateProjectionTransform();
    };

#endif // XCAMERA_H
