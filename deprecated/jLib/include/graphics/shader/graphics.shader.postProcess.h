#ifndef JLIB_GRAPHICS_SHADER_POSTPROCESS_H
#define JLIB_GRAPHICS_SHADER_POSTPROCESS_H

#include "graphics/shader/graphics.shader.types.h"
#include "graphics/shader/graphics.shader.surfaceShader.h"
#include "graphics/shader/graphics.shader.surfaceShaderVariableTypes.h"
#include "graphics/primitives/graphics.primitives.triangleCloud.h"

BEGIN_JLIB_GRAPHICS_SHADER_NAMESPACE

class postProcess : public primitives::triangleCloud, protected surfaceShader
    {
public:
    postProcess( entity::entityG *, string shader="" );
    void setSource( string );
    void rebuild();
    bool isValid();
    unsigned int sizeErrors();
    string getError( unsigned int );
    void clearErrors();
    void clearError( unsigned int );

    //sSampler2D &colour();
    //sSampler2D &depth();

private:
    //sSampler2D _colour;
    //sSampler2D _depth;
    };

END_JLIB_GRAPHICS_SHADER_NAMESPACE

#endif
