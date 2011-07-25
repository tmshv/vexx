#ifndef JLIB_GRAPHICS_SHADER_SURFACESHADERVARIABLETYPES_H
#define JLIB_GRAPHICS_SHADER_SURFACESHADERVARIABLETYPES_H

#include "graphics/shader/graphics.shader.surfaceShader.h"
#include "graphics/pixel/graphics.pixel.texture.h"
#include "graphics/shader/graphics.shader.light.h"
#include "QVector2D"
#include "QVector4D"

#define SHADERVARIABLE( n, t, glsl )                        \
class JLIB_EXPORT n : public surfaceShader::variable        \
    {                                                       \
    t val;                                                  \
public:                                                     \
    n( string name ) : surfaceShaderVariable( name )        \
        {                                                   \
        }                                                   \
    t operator()()                                          \
        {                                                   \
        return val;                                         \
        }                                                   \
    void operator=( t in )                                  \
        {                                                   \
        val = in;                                           \
        }                                                   \
    t get()                                                 \
        {                                                   \
        return val;                                         \
        }                                                   \
    void set( t in )                                        \
        {                                                   \
        val = in;                                           \
        }                                                   \
    virtual string glslType( ) const                        \
        {                                                   \
        return #glsl;                                       \
        }                                                   \
    virtual void set( );                                    \
    };


#define SHADERVARIABLESETSINGULAR( name, command )  \
void name::set( )                                   \
    {                                               \
    command( _varNum, val ) GLE;                    \
    }                                               \

#define SHADERVARIABLESETSINGLUARPOINTER( name, command, firstVal )     \
void name::set( )                                                       \
    {                                                                   \
    command( _varNum, 1, firstVal ) GLE;                                \
    }


BEGIN_JLIB_GRAPHICS_SHADER_NAMESPACE

SHADERVARIABLE( sBool, bool, bool )
SHADERVARIABLE( sFloat, float, float )
SHADERVARIABLE( sInt, int, int )
SHADERVARIABLE( sVec2, QVector2D, vec2 )
SHADERVARIABLE( sVec3, math::triple, vec3 )
SHADERVARIABLE( sVec4, QVector4D, vec4 )
SHADERVARIABLE( sColour, math::colour4, vec4 )
SHADERVARIABLE( sMat3x3, math::matrix3x3, mat3 )
SHADERVARIABLE( sMat4x3, math::matrix4x3, mat4x3 )
SHADERVARIABLE( sMatRef4x4, math::matrix4x4 *, mat4 )

class JLIB_EXPORT sSampler2D : public surfaceShader::variable
    {
    pixel::texture *val;
public:
    sSampler2D( string name ) : surfaceShaderVariable( name ), val( 0 )
        {
        }
    pixel::texture *operator()()
        {
        return val;
        }
    void operator=( pixel::texture *in )
        {
        val = in;
        }
    pixel::texture *get()
        {
        return val;
        }
    void set( pixel::texture *in )
        {
        val = in;
        }
    virtual string glslType( ) const
        {
        return "sampler2D";
        }
    virtual void set( );
    virtual void unset( );
    };

#if 0
class JLIB_EXPORT sLight : public surfaceShader::variable
    {
    light *val;
public:
    sLight( unsigned int id ) : surfaceShaderVariable( constructName( id ) )
        {
        }
    light *operator()()
        {
        return val;
        }
    void operator=( light &in )
        {
        val = &in;
        }
    light *get()
        {
        return val;
        }
    void set( light &in )
        {
        val = &in;
        }
    virtual string glslType( )
        {
        }

    virtual void set( );
    virtual void refreshLocation( int program );
private:
    int diffuseNum;
    int specularNum;
    int positionNum;
    int directionNum;
    int coneAngleNum;
    int coneCosFallOffNum;
    int constantAttenuationNum;
    int linearAttenuationNum;
    int quadraticAttenuationNum;
    string constructName( unsigned int id )
        {
        std::ostringstream str;
        str<<"lights["<<id<<"]";
        return str.str();
        }
    };
#endif

END_JLIB_GRAPHICS_SHADER_NAMESPACE

#endif
