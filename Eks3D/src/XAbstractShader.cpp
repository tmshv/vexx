#include "GL/glew.h"
#include "XAbstractShader.h"
#include "XAbstractRenderer.h"
#include "XAbstractTexture.h"
#include "QGLShaderProgram"

XGLShader::XGLShader( XGLRenderer *renderer ) : shader( renderer->context() )
    {
    }

void XGLShader::build( QString vertex, QString fragment )
    {
    shader.addShaderFromSourceCode( QGLShader::Vertex, vertex );
    shader.addShaderFromSourceCode( QGLShader::Fragment, fragment );
    shader.link();
    shader.bind();
    }

XAbstractShader::~XAbstractShader()
    {
    foreach( XAbstractShaderVariable *var, _variables )
        {
        delete var;
        }
    }

void XAbstractShader::setSources( QString vertex, QString fragment )
    {
    foreach( XAbstractShaderVariable *var, _variables )
        {
        var->rebind();
        }
    build( vertex, fragment );
    }

XAbstractShaderVariable *XAbstractShader::getVariable( QString in )
    {
    if( _variables.contains( in ) )
        {
        return _variables[in];
        }
    XAbstractShaderVariable *ret( createVariable( in ) );
    _variables.insert( in, ret );
    return ret;
    }

XMap <QString, XAbstractShaderVariable*> XAbstractShader::variables()
    {
    return _variables;
    }

XAbstractShaderVariable *XGLShader::createVariable( QString in )
    {
    return new XGLShaderVariable( this, in );
    }

XGLShaderVariable::XGLShaderVariable( XGLShader *p, QString name ) : _parent( p ), _name( name ), _texture( 0 )
    {
    rebind();
    }

void XGLShaderVariable::setValue( int value )
    {
    _texture = 0;
    _parent->shader.setUniformValue( _location, value );
    }

void XGLShaderVariable::setValue( xReal value )
    {
    _texture = 0;
    _parent->shader.setUniformValue( _location, value );
    }

void XGLShaderVariable::setValue( unsigned int value )
    {
    _texture = 0;
    _parent->shader.setUniformValue( _location, value );
    }

void XGLShaderVariable::setValue( const XColour &value )
    {
    _texture = 0;
    _parent->shader.setUniformValue( _location, value );
    }

void XGLShaderVariable::setValue( const XVector2D &value )
    {
    _texture = 0;
    _parent->shader.setUniformValue( _location, value );
    }

void XGLShaderVariable::setValue( const XVector3D &value )
    {
    _texture = 0;
    _parent->shader.setUniformValue( _location, value );
    }

void XGLShaderVariable::setValue( const XVector4D &value )
    {
    _texture = 0;
    _parent->shader.setUniformValue( _location, value );
    }

void XGLShaderVariable::setValue( const QMatrix2x2 &value )
    {
    _texture = 0;
    _parent->shader.setUniformValue( _location, value );
    }

void XGLShaderVariable::setValue( const QMatrix2x3 &value )
    {
    _texture = 0;
    _parent->shader.setUniformValue( _location, value );
    }

void XGLShaderVariable::setValue( const QMatrix2x4 &value )
    {
    _texture = 0;
    _parent->shader.setUniformValue( _location, value );
    }

void XGLShaderVariable::setValue( const QMatrix3x2 &value )
    {
    _texture = 0;
    _parent->shader.setUniformValue( _location, value );
    }

void XGLShaderVariable::setValue( const QMatrix3x3 &value )
    {
    _texture = 0;
    _parent->shader.setUniformValue( _location, value );
    }

void XGLShaderVariable::setValue( const QMatrix3x4 &value )
    {
    _texture = 0;
    _parent->shader.setUniformValue( _location, value );
    }

void XGLShaderVariable::setValue( const QMatrix4x2 &value )
    {
    _texture = 0;
    _parent->shader.setUniformValue( _location, value );
    }

void XGLShaderVariable::setValue( const QMatrix4x3 &value )
    {
    _texture = 0;
    _parent->shader.setUniformValue( _location, value );
    }

void XGLShaderVariable::setValue( const QMatrix4x4 &value )
    {
    _texture = 0;
    _parent->shader.setUniformValue( _location, value );
    }

void XGLShaderVariable::setValue( const XTexture *value )
    {
    _texture = static_cast<const XGLTexture*>(value->interface());
    _parent->shader.setUniformValue( _location, _texture->_id );
    }

void XGLShaderVariable::setValueArray( const XVector<int> &values )
    {
    _texture = 0;
    _parent->shader.setUniformValueArray( _location, &(values.front()), values.size() );
    }

void XGLShaderVariable::setValueArray( const XVector<xReal> &values )
    {
    _texture = 0;
    _parent->shader.setUniformValueArray( _location, &(values.front()), values.size(), 1 );
    }

void XGLShaderVariable::setValueArray( const XVector<unsigned int> &values )
    {
    _texture = 0;
    _parent->shader.setUniformValueArray( _location, &(values.front()), values.size() );
    }

void XGLShaderVariable::setValueArray( const XVector<XColour> &values )
    {
    _texture = 0;
    _parent->shader.setUniformValueArray( _location, &(values.front()), values.size() );
    }

void XGLShaderVariable::setValueArray( const XVector<XVector2D> &values )
    {
    _texture = 0;
    _parent->shader.setUniformValueArray( _location, &(values.front()), values.size() );
    }

void XGLShaderVariable::setValueArray( const XVector<XVector3D> &values )
    {
    _texture = 0;
    _parent->shader.setUniformValueArray( _location, &(values.front()), values.size() );
    }

void XGLShaderVariable::setValueArray( const XVector<XVector4D> &values )
    {
    _texture = 0;
    _parent->shader.setUniformValueArray( _location, &(values.front()), values.size() );
    }

void XGLShaderVariable::setValueArray( const XVector<QMatrix2x2> &values )
    {
    _texture = 0;
    _parent->shader.setUniformValueArray( _location, &(values.front()), values.size() );
    }

void XGLShaderVariable::setValueArray( const XVector<QMatrix2x3> &values )
    {
    _texture = 0;
    _parent->shader.setUniformValueArray( _location, &(values.front()), values.size() );
    }

void XGLShaderVariable::setValueArray( const XVector<QMatrix2x4> &values )
    {
    _texture = 0;
    _parent->shader.setUniformValueArray( _location, &(values.front()), values.size() );
    }

void XGLShaderVariable::setValueArray( const XVector<QMatrix3x2> &values )
    {
    _texture = 0;
    _parent->shader.setUniformValueArray( _location, &(values.front()), values.size() );
    }

void XGLShaderVariable::setValueArray( const XVector<QMatrix3x3> &values )
    {
    _texture = 0;
    _parent->shader.setUniformValueArray( _location, &(values.front()), values.size() );
    }

void XGLShaderVariable::setValueArray( const XVector<QMatrix3x4> &values )
    {
    _texture = 0;
    _parent->shader.setUniformValueArray( _location, &(values.front()), values.size() );
    }

void XGLShaderVariable::setValueArray( const XVector<QMatrix4x2> &values )
    {
    _texture = 0;
    _parent->shader.setUniformValueArray( _location, &(values.front()), values.size() );
    }

void XGLShaderVariable::setValueArray( const XVector<QMatrix4x3> &values )
    {
    _texture = 0;
    _parent->shader.setUniformValueArray( _location, &(values.front()), values.size() );
    }

void XGLShaderVariable::setValueArray( const XVector<QMatrix4x4> &values )
    {
    _texture = 0;
    _parent->shader.setUniformValueArray( _location, &(values.front()), values.size() );
    }

void XGLShaderVariable::rebind()
    {
    _location = _parent->shader.uniformLocation( _name );
    }
