#include "graphics/shader/graphics.shader.tools.h"
#include "jLib.glHeader.h"

BEGIN_JLIB_GRAPHICS_SHADER_NAMESPACE

namespace tools
{
bool compile( int shaderNum, string source, string& logOut )
    {
    int compileStatus;
    const char *cSource = source.c_str();

    // Source and compile shader
    glShaderSource( shaderNum, 1, &cSource, NULL );
    glCompileShader( shaderNum );

    // Test for success
    glGetShaderiv( shaderNum, GL_COMPILE_STATUS, &compileStatus );
    if( compileStatus == GL_TRUE )
        {
        //Success
        logOut = "";
        return true;
        }

    // Failed, return the log from the compiler
    int logLength;
    glGetShaderiv( shaderNum, GL_INFO_LOG_LENGTH, &logLength );
    char *log = new char[logLength];
    glGetShaderInfoLog(	shaderNum, logLength, NULL, log );
    logOut = log;

    delete [] log;
    return false;
    }

bool link( int &programNum, const int *shdrs, unsigned int num, string &err )
    {
    programNum = glCreateProgram();
    if( programNum != 0 )
        {
        int compileStatus;

        for( unsigned int x=0; x<num; x++ )
            {
            glAttachShader( programNum, shdrs[x] );
            }
        glLinkProgram( programNum );

        // test for link success
        glGetProgramiv( programNum, GL_LINK_STATUS, &compileStatus );
        if( compileStatus == GL_TRUE )
            {
            //Success
            err = "";
            return true;
            }

        // Failed, return the log from the compiler
        int logLength;
        glGetProgramiv( programNum, GL_INFO_LOG_LENGTH, &logLength );
        char *log = new char[logLength];
        glGetProgramInfoLog( programNum, logLength, NULL, log );

        err = log;
        delete [] log;
        }
    return false;
    }
}

END_JLIB_GRAPHICS_SHADER_NAMESPACE
