#ifndef GRAPHICS_SHADER_LIGHTINGBASE_H
#define GRAPHICS_SHADER_LIGHTINGBASE_H

#include "graphics/shader/graphics.shader.types.h"
#include "graphics/shader/graphics.shader.surfaceShader.h"

BEGIN_JLIB_GRAPHICS_SHADER_NAMESPACE

#define JLIB_LIGHT_STRUCT "struct light"    \
"{"                                         \
"vec3 ambient;"                             \
"vec3 diffuse;"                             \
"vec3 specular;"                            \
"vec3 position;"                            \
"vec3 direction;"                           \
"float coneAngle;"                          \
"float coneFallOff;"                        \
"float constantAttenuation;"                \
"float linearAttenuation;"                  \
"float quadraticAttenuation;"               \
"};"

#define JLIB_SHADER_DEFS( lights )          \
"uniform struct light lights[" #lights "];" \
"varying vec4 position;"                    \
"varying vec3 normal;"                      \

#define JLIB_FRAG_DEFS( lights )            \
JLIB_SHADER_DEFS( lights )                  \
"float distanceToLight( int l )"            \
"{"                                         \
"return distance("                          \
"       vec4( lights[l].position, 1 ),"     \
"       position );"                        \
"}"                                         \
"vec3 ambient( int l, float dist )"         \
"{"                                         \
"return lights[l].ambient * ( 1.0 / ( lights[l].constantAttenuation + " \
"                                      lights[l].linearAttenuation * dist + " \
"                                      lights[l].quadraticAttenuation * dist * dist ) ).rrr;" \
"}"                                         \
"vec3 totalAmbient( )"                      \
"{"                                         \
"vec3 ret = 0.0;"                           \
"int x;"                                    \
"for( x=0; x<lights; x++ )"                 \
"   {"                                      \
"   ret += ambient( x, distanceToLight( x ) );" \
"   }"                                      \
"return ret;"                               \
"}"

#define JLIB_BASIC_VERTEX( extras )             \
"void main()"                                   \
"{"                                             \
"position = gl_ModelViewMatrix * gl_Vertex;"    \
"normal = gl_NormalMatrix * gl_Normal;"         \
"gl_Position = ftransform();"                   \
extras                                          \
"}"

#define JLIB_VERT_DEFS( lights )                \
JLIB_SHADER_DEFS( lights )

class lightingBase : public surfaceShader
    {
    jProperty( unsigned int, numLights )

protected:
    lightingBase( string, string );
    void setSource( string, string );
    };

END_JLIB_GRAPHICS_SHADER_NAMESPACE

#endif
