#ifndef XABSTRACTTEXTURE_H
#define XABSTRACTTEXTURE_H

#include "XObject"

class QImage;
class XGLRenderer;
class XAbstractRenderer;

class XAbstractTexture
    {
public:
    virtual void load( const QImage & ) = 0;
    };

class XTexture : public XObject
    {
public:
    XTexture( XAbstractRenderer *renderer );
    XTexture( XAbstractRenderer *renderer, QImage );
    ~XTexture( );

    void load( const QImage & );

    XAbstractTexture *interface() const;

private:
    XAbstractTexture *_internal;
    };

class XGLTexture : public XAbstractTexture
    {
public:
    XGLTexture( XGLRenderer * );
    ~XGLTexture();
    virtual void load( const QImage & );

private:
    void clear();
    XGLRenderer *_renderer;
    unsigned int _id;
    friend class XGLShaderVariable;
    friend class XGLRenderer;
    };

#endif // XABSTRACTTEXTURE_H
