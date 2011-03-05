#include "graphics/shader/graphics.shader.lightingBase.h"

BEGIN_JLIB_GRAPHICS_SHADER_NAMESPACE

lightingBase::lightingBase( string frag, string vert ) :  _numLights( 1 )
    {
    clearErrors();
    setSource( frag, vert );
    rebuild();
    jAssert( isValid() );
    }

void lightingBase::setSource( string frag, string vert )
    {
    surfaceShader::setSource( JLIB_FRAG_DEFS( 1 ) + frag, JLIB_VERT_DEFS( 1 ) + vert );
    }

END_JLIB_GRAPHICS_SHADER_NAMESPACE
