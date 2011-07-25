#include "jLib.glHeader.h"

BEGIN_JLIB_NAMESPACE

void glewInitiate()
    {
    glewInit();
    }

string handleGLError( int err )
    {
    if( err == GL_INVALID_ENUM )
        {
        return "GL Error: Invalid Enum";
        }
    else if( err == GL_INVALID_VALUE )
        {
        return "GL Error: Invalid Value";
        }
    else if( err == GL_INVALID_OPERATION )
        {
        return "GL Error: Invalid Operation";
        }
    else if( err == GL_STACK_OVERFLOW )
        {
        return "GL Error: Stack Overflow";
        }
    else if( err == GL_STACK_UNDERFLOW )
        {
        return "GL Error: Stack Underflow";
        }
    else if( err == GL_OUT_OF_MEMORY )
        {
        return "GL Error: Out Of Memory";
        }
    return "GL Error: No Error";
    }

END_JLIB_NAMESPACE
