#ifndef XABSTRACTSHADER_H
#define XABSTRACTSHADER_H

#include "QByteArray"
#include "QGLShaderProgram"
#include "XColour"
#include "XVector"
#include "XVector2D"
#include "XVector3D"
#include "XVector4D"
#include "XMap"

class XTexture;
class XGLTexture;

class XAbstractShaderVariable
    {
public:
    virtual void setValue( int value ) = 0;
    virtual void setValue( xReal value ) = 0;
    virtual void setValue( unsigned int value ) = 0;
    virtual void setValue( const XColour &color ) = 0;
    virtual void setValue( const XVector2D &value ) = 0;
    virtual void setValue( const XVector3D &value ) = 0;
    virtual void setValue( const XVector4D &value ) = 0;
    virtual void setValue( const QMatrix2x2 &value ) = 0;
    virtual void setValue( const QMatrix2x3 &value ) = 0;
    virtual void setValue( const QMatrix2x4 &value ) = 0;
    virtual void setValue( const QMatrix3x2 &value ) = 0;
    virtual void setValue( const QMatrix3x3 &value ) = 0;
    virtual void setValue( const QMatrix3x4 &value ) = 0;
    virtual void setValue( const QMatrix4x2 &value ) = 0;
    virtual void setValue( const QMatrix4x3 &value ) = 0;
    virtual void setValue( const QMatrix4x4 &value ) = 0;
    virtual void setValue( const XTexture *value ) = 0;
    virtual void setValueArray( const XVector<int> &values ) = 0;
    virtual void setValueArray( const XVector<xReal> &values ) = 0;
    virtual void setValueArray( const XVector<unsigned int> &values ) = 0;
    virtual void setValueArray( const XVector<XColour> &values ) = 0;
    virtual void setValueArray( const XVector<XVector2D> &values ) = 0;
    virtual void setValueArray( const XVector<XVector3D> &values ) = 0;
    virtual void setValueArray( const XVector<XVector4D> &values ) = 0;
    virtual void setValueArray( const XVector<QMatrix2x2> &values ) = 0;
    virtual void setValueArray( const XVector<QMatrix2x3> &values ) = 0;
    virtual void setValueArray( const XVector<QMatrix2x4> &values ) = 0;
    virtual void setValueArray( const XVector<QMatrix3x2> &values ) = 0;
    virtual void setValueArray( const XVector<QMatrix3x3> &values ) = 0;
    virtual void setValueArray( const XVector<QMatrix3x4> &values ) = 0;
    virtual void setValueArray( const XVector<QMatrix4x2> &values ) = 0;
    virtual void setValueArray( const XVector<QMatrix4x3> &values ) = 0;
    virtual void setValueArray( const XVector<QMatrix4x4> &values ) = 0;

    virtual void rebind() = 0;
    };

class XAbstractShader
    {
public:
    virtual ~XAbstractShader();

    virtual void setSources( QString vertex, QString fragment );
    XAbstractShaderVariable *getVariable( QString );

    XMap <QString, XAbstractShaderVariable*> variables();

protected:
    virtual void build( QString vertex, QString fragment ) = 0;
private:
    virtual XAbstractShaderVariable *createVariable( QString ) = 0;
    XMap <QString, XAbstractShaderVariable*> _variables;
    };


class XGLRenderer;
class XGLShader : public XAbstractShader
    {
public:
    XGLShader( XGLRenderer * );

private:
    void build( QString vertex, QString fragment );
    virtual XAbstractShaderVariable *createVariable( QString );

    QGLShaderProgram shader;
    friend class XGLRenderer;
    friend class XGLShaderVariable;
    };

class XGLShaderVariable : public XAbstractShaderVariable
    {
public:
    XGLShaderVariable( XGLShader *, QString );

    void setValue( int value );
    void setValue( xReal value );
    void setValue( unsigned int value );
    void setValue( const XColour &value );
    void setValue( const XVector2D &value );
    void setValue( const XVector3D &value );
    void setValue( const XVector4D &value );
    void setValue( const QMatrix2x2 &value );
    void setValue( const QMatrix2x3 &value );
    void setValue( const QMatrix2x4 &value );
    void setValue( const QMatrix3x2 &value );
    void setValue( const QMatrix3x3 &value );
    void setValue( const QMatrix3x4 &value );
    void setValue( const QMatrix4x2 &value );
    void setValue( const QMatrix4x3 &value );
    void setValue( const QMatrix4x4 &value );
    void setValue( const XTexture *value );
    void setValueArray( const XVector<int> &values );
    void setValueArray( const XVector<xReal> &values );
    void setValueArray( const XVector<unsigned int> &values );
    void setValueArray( const XVector<XColour> &values );
    void setValueArray( const XVector<XVector2D> &values );
    void setValueArray( const XVector<XVector3D> &values );
    void setValueArray( const XVector<XVector4D> &values );
    void setValueArray( const XVector<QMatrix2x2> &values );
    void setValueArray( const XVector<QMatrix2x3> &values );
    void setValueArray( const XVector<QMatrix2x4> &values );
    void setValueArray( const XVector<QMatrix3x2> &values );
    void setValueArray( const XVector<QMatrix3x3> &values );
    void setValueArray( const XVector<QMatrix3x4> &values );
    void setValueArray( const XVector<QMatrix4x2> &values );
    void setValueArray( const XVector<QMatrix4x3> &values );
    void setValueArray( const XVector<QMatrix4x4> &values );

    virtual void rebind();
private:
    XGLShader *_parent;
    QString _name;
    int _location;
    const XGLTexture *_texture;
    friend class XGLRenderer;
    };

#endif // XABSTRACTSHADER_H
