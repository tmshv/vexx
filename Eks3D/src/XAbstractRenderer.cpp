#include "GL/glew.h"
#include "XAbstractRenderer.h"
#include "XAbstractGeometryCache.h"
#include "XAbstractShader.h"
#include "XAbstractTexture.h"
#include "XDebug"

QGLFormat fmt( bool multi )
    {
    QGLFormat ret;
    ret.setSampleBuffers( multi );
    return ret;
    }

XGLRenderer::XGLRenderer( bool multi ) : _context( new QGLContext( fmt( multi ) ) ), _currentShader( 0 )
    {
    }

QGLContext *XGLRenderer::context()
    {
    return _context;
    }

const QGLContext *XGLRenderer::context() const
    {
    return _context;
    }

void XGLRenderer::intialise()
    {
    glewInit();
    glEnable( GL_DEPTH_TEST );
    }

void XGLRenderer::pushTransform( const XTransform &trans )
    {
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glMultMatrixd( trans.constData() );
    }

void XGLRenderer::popTransform( )
    {
    glMatrixMode( GL_MODELVIEW );
    glPopMatrix();
    }

void XGLRenderer::clear( )
    {
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    }

void XGLRenderer::setViewportSize( QSize size )
    {
    glViewport( 0, 0, size.width(), size.height() );
    }

void XGLRenderer::setProjectionTransform( const XTransform &trans )
    {
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glMultMatrixd( trans.constData() );
    }

void XGLRenderer::drawGeomtreyCache( XAbstractGeometryCache *cache )
    {
    XList<uint> ids;
    XGLGeometryCache *gC = static_cast<XGLGeometryCache*>(cache);

    glEnableClientState( GL_VERTEX_ARRAY );

    foreach( QString name, gC->_attr1.keys() )
        {
        XVector<xReal> vec( gC->_attr1.value( name ) );

        uint id( _currentShader->shader.attributeLocation( name ) );

        glEnableVertexAttribArray( id );
        glVertexAttribPointer( id, 1, GL_FLOAT, GL_FALSE, 0, &(vec.front()) );

        ids << id;
        }
    foreach( QString name, gC->_attr2.keys() )
        {
        XVector<XAbstractGeometryCache::Vertex2D> vec( gC->_attr2.value( name ) );

        uint id( _currentShader->shader.attributeLocation( name ) );

        glEnableVertexAttribArray( id );
        glVertexAttribPointer( id, 2, GL_FLOAT, GL_FALSE, 0, &(vec.front()) );

        ids << id;
        }
    foreach( QString name, gC->_attr3.keys() )
        {
        XVector<XAbstractGeometryCache::Vertex3D> vec( gC->_attr3.value( name ) );

        uint id( _currentShader->shader.attributeLocation( name ) );

        glEnableVertexAttribArray( id );
        glVertexAttribPointer( id, 3, GL_FLOAT, GL_FALSE, 0, &(vec.front()) );

        ids << id;
        }
    foreach( QString name, gC->_attr4.keys() )
        {
        XVector<XAbstractGeometryCache::Vertex4D> vec( gC->_attr4.value( name ) );

        uint id( _currentShader->shader.attributeLocation( name ) );

        glEnableVertexAttribArray( id );
        glVertexAttribPointer( id, 4, GL_FLOAT, GL_FALSE, 0, &(vec.front()) );

        ids << id;
        }

    glVertexPointer( 3, GL_FLOAT, 0, &(gC->_vertices.front().a) );

    int points( gC->_points.size() );
    if( points )
        {
        glDrawElements( GL_POINTS, points, GL_UNSIGNED_INT, &(gC->_points.front()) );
        }

    int lines( gC->_lines.size() );
    if( lines )
        {
        glDrawElements( GL_LINES, lines*2, GL_UNSIGNED_INT, &(gC->_lines.front().a) );
        }

    int triangles( gC->_triangles.size() );
    if( triangles )
        {
        glDrawElements( GL_TRIANGLES, triangles*3, GL_UNSIGNED_INT, &(gC->_triangles.front().a) );
        }

    foreach( uint id, ids )
        {
        glDisableVertexAttribArray( id );
        }

    glDisableClientState( GL_VERTEX_ARRAY );
    }

XAbstractGeometryCache *XGLRenderer::getGeometryCache()
    {
    return new XGLGeometryCache;
    }

void XGLRenderer::setShader( XAbstractShader *shader )
    {
    if( shader )
        {
        xAssert( !_currentShader );
        _currentShader = static_cast<XGLShader*>(shader);

        _currentShader->shader.bind();

        int x=0;
        foreach( XAbstractShaderVariable *var, shader->variables() )
            {
            XGLShaderVariable *glVar( static_cast<XGLShaderVariable*>(var) );
            const XGLTexture *tex( glVar->_texture );
            glActiveTexture( x );
            glBindTexture( GL_TEXTURE_2D, tex->_id );
            _currentShader->shader.setUniformValue( glVar->_location, x );
            x++;
            }
        }
    else
        {
        _currentShader->shader.release();
        _currentShader = 0;
        }
    }

XAbstractShader *XGLRenderer::getShader( QString v, QString f )
    {
    XGLShader *ret( new XGLShader( this ) );

    ret->setSources( v, f );

    return ret;
    }

XAbstractTexture *XGLRenderer::getTexture()
    {
    return new XGLTexture( this );
    }
