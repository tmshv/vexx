#include "XAbstractTexture.h"
#include "XAbstractRenderer.h"
#include "QGLContext"
#include "QImage"

#include "XDebug"

XGLTexture::XGLTexture( XGLRenderer *r ) : _renderer( r ), _id( 0 )
    {
    }

XGLTexture::~XGLTexture()
    {
    clear();
    }

XTexture::XTexture( XAbstractRenderer *renderer ) : _internal( renderer->getTexture() )
    {
    }

XTexture::XTexture( XAbstractRenderer *renderer, QImage im ) : _internal( renderer->getTexture() )
    {
    load( im );
    }

XTexture::~XTexture( )
    {
    delete _internal;
    }

void XTexture::load( const QImage &im )
    {
    if( !im.isNull() )
        {
        _internal->load( im );
        }
    }

XAbstractTexture *XTexture::interface() const
    {
    return _internal;
    }

void XGLTexture::load( const QImage &im )
    {
    clear();
    _id = _renderer->context()->bindTexture( im );
    }

void XGLTexture::clear()
    {
    _renderer->context()->deleteTexture( _id );
    }
