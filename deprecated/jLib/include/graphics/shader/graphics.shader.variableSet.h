#ifndef JLIB_GRAPHICS_SHADER_VARIABLESET_H
#define JLIB_GRAPHICS_SHADER_VARIABLESET_H

#include "graphics/shader/graphics.shader.types.h"
#include "graphics/shader/graphics.shader.surfaceShader.h"

BEGIN_JLIB_GRAPHICS_SHADER_NAMESPACE

class JLIB_EXPORT variableSet
    {
public:
    void setShader( surfaceShader & );
    void setValues();
    void unsetValues();
    bool add( surfaceShader::variable & );
    bool remove( string );
    void refreshLocations( );
    void clear( );
private:
    surfaceShader *_shader;
    vector <surfaceShader::variable *> vars;
    JLIB_VECTOR_CHILD( vector <surfaceShader::variable *>, vars )
    };

END_JLIB_GRAPHICS_SHADER_NAMESPACE

#endif
