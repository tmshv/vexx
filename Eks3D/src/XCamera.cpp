#include "XCamera.h"
#include "XScene.h"
#include "XDebug"

XCamera::XCamera( XVector3D position, XVector3D aimPosition, XVector3D up )
        : _position( position ), _aimPosition( aimPosition ), _upDirection( up ),
        _currentScene( 0 ), _invertedTransformIsValid(false)
    {
    setTransform();
    }

void XCamera::setUpDirection( XVector3D up )
    {
    _upDirection = up;
    setTransform();
    }

void XCamera::setPosition( XVector3D pos )
    {
    _position = pos;
    setTransform();
    }

void XCamera::setAimDirection( XVector3D aim )
    {
    _aimPosition = position() + aim;
    setTransform();
    }

void XCamera::setAimPosition( XVector3D aim )
    {
    _aimPosition = aim;
    setTransform();
    }

XVector3D XCamera::aimDirection() const
    {
    return _viewTransform.row( 2 ).toVector3D();
    }

XVector3D XCamera::vertical() const
    {
    return _viewTransform.row( 1 ).toVector3D();
    }

XVector3D XCamera::horizontal() const
    {
    return _viewTransform.row( 0 ).toVector3D();
    }

void XCamera::pan( xReal x, xReal y )
    {
    setAimPosition( position() + ( ( aimPosition() - position() ) + ( horizontal() * x ) + ( vertical() * y ) ) );
    }

void XCamera::track( xReal x, xReal y, xReal z )
    {
    XVector3D adj( x * horizontal() + y * vertical() + z * aimDirection() );

    setPosition( position() + adj );
    setAimPosition( aimPosition() + adj );
    }

void XCamera::setViewportSize( const QSize &in )
    {
    _aspectRatio = (float)in.width() / (float)in.height();
    _viewportSize = in;

    aspectRatioChanged();
    }

XVector3D XCamera::screenToWorld( const QPoint &pos ) const
    {
    QPointF posRatio((2.0f*((float)pos.x()/(float)_viewportSize.width()))-1.0f,
                -1.0f*((2.0f*((float)pos.y()/(float)_viewportSize.height()))-1.0f));

    if(!_invertedTransformIsValid)
      {
      _inverted = viewTransform().inverted() * projectionTransform().inverted();
      }

    return _inverted * XVector3D(posRatio);
    }

void XCamera::setProjectionTransform( XTransform proj )
    {
    _projectionTransform = proj;

    _invertedTransformIsValid = false;
    trigger(projectionChanged());
    if( currentScene() )
        {
        trigger( currentScene()->update() );
        }
    }

void XCamera::aspectRatioChanged()
    {
    }

void XCamera::viewTransformChanged()
    {
    }

void XCamera::setTransform()
    {
    _viewTransform = XTransform();

    _viewTransform.lookAt( position(), aimPosition(), upDirection() );

    _invertedTransformIsValid = false;
    viewTransformChanged();
    trigger(viewChanged());
    if( currentScene() )
        {
        trigger( currentScene()->update() );
        }
    }

XPerspectiveCamera::XPerspectiveCamera( xReal angle, XVector3D position, XVector3D aimPosition, XVector3D up, xReal near, xReal far )
        : XCamera( position, aimPosition, up ), _viewAngle( angle ), _nearClipPlane( near ), _farClipPlane( far )
    {
    updateProjectionTransform();
    }

void XPerspectiveCamera::setViewAngle( xReal in )
    {
    _viewAngle = in;
    updateProjectionTransform();
    }

void XPerspectiveCamera::setNearClipPlane( xReal in )
    {
    _nearClipPlane = in;
    updateProjectionTransform();
    }

void XPerspectiveCamera::setFarClipPlane( xReal in )
    {
    _farClipPlane = in;
    updateProjectionTransform();
    }

void XPerspectiveCamera::aspectRatioChanged()
    {
    updateProjectionTransform();
    }

void XPerspectiveCamera::viewTransformChanged()
    {
    _frustum = XFrustum(position()*-1.0, aimDirection(), horizontal(), vertical(), viewAngle(), aspectRatio(), nearClipPlane(), farClipPlane());
    }

void XPerspectiveCamera::updateProjectionTransform()
    {
    XTransform mat;
    mat.perspective( viewAngle(), aspectRatio(), nearClipPlane(), farClipPlane() );
    setProjectionTransform( mat );
    }
