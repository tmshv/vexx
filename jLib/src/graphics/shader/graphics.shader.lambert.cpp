#include "graphics/shader/graphics.shader.lambert.h"

BEGIN_JLIB_GRAPHICS_SHADER_NAMESPACE

#define JLIB_LAMBERT_FRAG               \
"void main( )"                          \
"{"                                     \
"gl_FragColor = vec4( 1, 0, 0, 1 );"       \
"}"

lambert::lambert( ) : lightingBase( JLIB_LAMBERT_FRAG, JLIB_BASIC_VERTEX("") )
    {
    }

END_JLIB_GRAPHICS_SHADER_NAMESPACE
