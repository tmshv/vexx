#ifndef XRENDERER_H
#define XRENDERER_H

#include "X3DGlobal.h"
#include "XObject"
#include "XTransform.h"

class XAbstractGeometryCache;
class XAbstractShader;
class XAbstractTexture;

class EKS3D_EXPORT XAbstractRenderer : public XObject
    {
    X_OBJECT( XRenderer, XObject, 4 )

public:
    virtual void pushTransform( const XTransform & ) = 0;
    virtual void popTransform( ) = 0;

    virtual void clear() = 0;

    virtual void setViewportSize( QSize ) = 0;
    virtual void setProjectionTransform( const XTransform & ) = 0;

    virtual XAbstractShader *getShader( QString vertex, QString fragment ) = 0;
    virtual void setShader( XAbstractShader * ) = 0;

    virtual void drawGeomtreyCache( XAbstractGeometryCache * ) = 0;
    virtual XAbstractGeometryCache *getGeometryCache() = 0;

    virtual XAbstractTexture *getTexture() = 0;
    };

class XGLShader;
class QGLContext;
class EKS3D_EXPORT XGLRenderer : public XAbstractRenderer
    {
public:
    XGLRenderer( bool multiSample=false );

    QGLContext *context();
    const QGLContext *context() const;

    void intialise();

    virtual void pushTransform( const XTransform & );
    virtual void popTransform( );

    virtual void clear();

    virtual void setViewportSize( QSize );
    virtual void setProjectionTransform( const XTransform & );

    virtual XAbstractShader *getShader( QString vertex, QString fragment );
    virtual void setShader( XAbstractShader * );

    virtual void drawGeomtreyCache( XAbstractGeometryCache * );
    virtual XAbstractGeometryCache *getGeometryCache();

    virtual XAbstractTexture *getTexture();

private:
    QGLContext *_context;
    XGLShader *_currentShader;
    };

#endif // XRENDERER_H
