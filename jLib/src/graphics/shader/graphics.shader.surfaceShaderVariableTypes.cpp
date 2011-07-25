#include "graphics/shader/graphics.shader.surfaceShaderVariableTypes.h"
#include "jLib.glHeader.h"

BEGIN_JLIB_GRAPHICS_SHADER_NAMESPACE

#define JLIB_SET_VEC2( num, trip ) glUniform2f( num, trip.x(), trip.y() ) GLE;
#define JLIB_SET_VEC3( num, trip ) glUniform3f( num, trip.x(), trip.y(), trip.z() ) GLE;
#define JLIB_SET_VEC4( num, trip ) glUniform4f( num, trip.x(), trip.y(), trip.z(), trip.w() ) GLE;

SHADERVARIABLESETSINGULAR( sBool, glUniform1i )
SHADERVARIABLESETSINGULAR( sInt, glUniform1i )
SHADERVARIABLESETSINGULAR( sFloat, glUniform1f )
//SHADERVARIABLESETSINGLUARPOINTER( sVec3, glUniform3fv, val.ptr() )

void sVec2::set( )
    {
    JLIB_SET_VEC2( _varNum, val );
    }
void sVec3::set( )
    {
    JLIB_SET_VEC3( _varNum, val );
    }
void sVec4::set( )
    {
    JLIB_SET_VEC4( _varNum, val );
    }

void sColour::set( )
    {
    glUniform4f( _varNum, val.r(), val.g(), val.b(), val.a() ) GLE;
    }

void sMat3x3::set( )
    {
    glUniformMatrix3fv( _varNum, 1, 0, val.ptr() ) GLE;
    }

void sMat4x3::set( )
    {
    float temp[12];
    for( int x=0; x<12; x++ )
        {
        temp[x] = val.ptr()[x];
        }
    glUniformMatrix3x4fv( _varNum, 1, 0, temp ) GLE;
    }

void sMatRef4x4::set( )
    {
    float temp[16];
    for( int x=0; x<16; x++ )
        {
        temp[x] = val->ptr()[x];
        }
    glUniformMatrix4fv( _varNum, 1, 1, temp ) GLE;
    }

void sSampler2D::set( )
    {
    if( val )
        {
        //jDebugNeat<<val<<" Link and bind texture "<<val->textureID()<<" to unit "<<val->textureUnit()<<", to var "<<_varNum<<endl;
        jRenderDebug( val<<" Link and bind texture "<<val->textureID()<<" to unit "<<val->textureUnit()<<", to var "<<_varNum );
        glUniform1i( _varNum, val->textureUnit() ) GLE;
        val->bind() GLE;
        }
    }

void sSampler2D::unset( )
    {
    if( val )
        {
        //jDebugNeat<<val << " unbind texture from shader" << endl;
        jRenderDebug( val << " unbind texture from shader" );
        val->unbind() GLE;
        }
    }

#if 0
void sLight::set( )
    {
    JLIB_SET_VEC3( _varNum, val->ambient() );
    JLIB_SET_VEC3( diffuseNum, val->diffuse() );
    JLIB_SET_VEC3( specularNum, val->specular() );
    JLIB_SET_VEC3( positionNum, ( val->camera()->view().pointMultiply( val->position() ) ) );
    JLIB_SET_VEC3( directionNum, ( val->camera()->view().vectorMultiply( val->direction() ) ) );
    glUniform1f( coneAngleNum, val->coneAngle() ) GLE;
    glUniform1f( coneCosFallOffNum, val->coneCosFallOff() ) GLE;
    glUniform1f( constantAttenuationNum, val->constantAttenuation() ) GLE;
    glUniform1f( linearAttenuationNum, val->linearAttenuation() ) GLE;
    glUniform1f( quadraticAttenuationNum, val->quadraticAttenuation() ) GLE;
    }

void sLight::refreshLocation( int program )
    {
    string n = name();
    _varNum = glGetUniformLocation( program, ( _name + ".ambient" ).c_str() ) GLE;
    diffuseNum = glGetUniformLocation( program, ( _name + ".diffuse" ).c_str() ) GLE;
    specularNum = glGetUniformLocation( program, ( _name + ".specular" ).c_str() ) GLE;
    positionNum = glGetUniformLocation( program, ( _name + ".position" ).c_str() ) GLE;
    directionNum = glGetUniformLocation( program, ( _name + ".direction" ).c_str() ) GLE;
    coneAngleNum = glGetUniformLocation( program, ( _name + ".coneAngle" ).c_str() ) GLE;
    coneCosFallOffNum = glGetUniformLocation( program, ( _name + ".coneCosFallOff" ).c_str() ) GLE;
    constantAttenuationNum = glGetUniformLocation( program, ( _name + ".constantAttenuation" ).c_str() ) GLE;
    linearAttenuationNum = glGetUniformLocation( program, ( _name + ".linearAttenuation" ).c_str() ) GLE;
    quadraticAttenuationNum = glGetUniformLocation( program, ( _name + ".quadraticAttenuation" ).c_str() ) GLE;
    }
#endif
END_JLIB_GRAPHICS_SHADER_NAMESPACE
