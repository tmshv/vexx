#include "graphics/shader/graphics.shader.surfaceShader.h"
#include "graphics/shader/graphics.shader.tools.h"
#include "jLib.glHeader.h"

BEGIN_JLIB_GRAPHICS_SHADER_NAMESPACE

surfaceShader::surfaceShader( string fragmentShader, string vertexShader ) : _fragNum( 0 ),
        _vertNum( 0 ), _programNum( 0 )
    {
    jAssert( glewIsSupported("GL_VERSION_2_0") && "Ensure you have tried to create the shader while the gl context is active." );

    setFragmentSource( fragmentShader );
    setVertexSource( vertexShader );
    if( isValidForFragmentCompile() )
        {
        rebuild();
        }
    else
        {
        _errors.push_back( "Shader isn't valid for compile ( Fragment source may be empty )." );
        }
    }

void surfaceShader::variable::setName( string n )
    {
    _name = n;
    }

string surfaceShader::variable::name( ) const
    {
    return _name;
    }

surfaceShaderVariable::surfaceShaderVariable( string n )
    {
    _name = n;
    _varNum = -1;
    }

surfaceShaderVariable::~surfaceShaderVariable()
    {
    }

void surfaceShader::variable::refreshLocation( int program )
    {
    glUseProgram( program ) GLE;
    jRenderDebug( "Refresh Location for variable "<<_name<<" in program "<<program<<endl<<"old value "<<_varNum );
    _varNum = glGetUniformLocation( program, _name.c_str() ) GLE;
    jRenderDebug( "New value "<<_varNum );
    glUseProgram( 0 ) GLE;
    }

void surfaceShader::setFragmentSource( string frag )
    {
    _fragment = frag;
    }

void surfaceShader::setVertexSource( string vert )
    {
    _vertex = vert;
    }

void surfaceShader::setSource( string frag, string vert )
    {
    setFragmentSource( frag );
    setVertexSource( vert );
    }

void surfaceShader::rebuild()
    {
    clearShader();
    if( isValidForFragmentCompile() )
        {
        compileFragment();

        if( this->sizeErrors() )
            {
            return;
            }

        if( isValidForVertexCompile() )
            {
            compileVertex();

            if( this->sizeErrors() )
                {
                return;
                }
            }
        linkProgram();
        }
    }

string surfaceShader::getFragmentSource( )
    {
    return _fragment;
    }

string surfaceShader::getVertexSource( )
    {
    return _vertex;
    }

string surfaceShader::getError( unsigned int x )
    {
    jAssert( x < _errors.size() );
    return _errors[x];
    }
void surfaceShader::clearErrors()
    {
    _errors.clear();
    }
void surfaceShader::clearError( unsigned int x )
    {
    _errors.erase( _errors.begin() + x );
    }

unsigned int surfaceShader::sizeErrors()
    {
    return _errors.size();
    }

bool surfaceShader::isValid()
    {
    if( !glIsProgram( _programNum ) || _programNum == 0 )
        {
        return false;
        }

    glUseProgram( _programNum );
    if( glGetError() )
        {
        return false;
        }
    glUseProgram( 0 );

    return true;
    }

void surfaceShader::use()
    {
    jAssert( isValid() );
    glUseProgram( _programNum ) GLE;
    }

void surfaceShader::clear()
    {
    glUseProgram( 0 ) GLE;
    }

bool surfaceShader::compileFragment()
    {
    jAssert( _fragment != "" );
    jAssert( _fragNum == 0 );
    clearFragment();
    string error;

    _fragNum = glCreateShader( GL_FRAGMENT_SHADER );
    if( !tools::compile( _fragNum, _fragment, error ) )
        {
        _errors.push_back( error );
        return false;
        }
    return true;
    }

bool surfaceShader::compileVertex()
    {
    jAssert( _vertex != "" );
    jAssert( _vertNum == 0 );
    clearVertex();
    string error;

    _vertNum = glCreateShader( GL_VERTEX_SHADER );
    if( !tools::compile( _vertNum, _vertex, error ) )
        {
        _errors.push_back( error );
        return false;
        }
    return true;
    }

bool surfaceShader::linkProgram()
    {
    clearProgram();
    jAssert( _fragNum != 0 );

    string error;
    unsigned int num = 1;
    int shaders[2];
    shaders[0] = _fragNum;
    if( _vertNum != 0 )
        {
        shaders[1] = _vertNum;
        num++;
        }

    if( !tools::link( _programNum, shaders, num, error ) )
        {
        _errors.push_back( error );
        return false;
        }
    return true;
    }

bool surfaceShader::isValidForFragmentCompile()
    {
    return _fragment != "";
    }

bool surfaceShader::isValidForVertexCompile()
    {
    return _vertex != "";
    }

void surfaceShader::clearShader()
    {
    clearFragment();
    clearVertex();
    clearProgram();
    }

void surfaceShader::clearProgram()
    {
    if( _programNum != 0 )
        {
        glDeleteProgram( _programNum );
        _programNum = 0;
        }
    }

void surfaceShader::clearFragment()
    {
    if( _fragNum != 0 )
        {
        if( _programNum != 0 )
            {
            glDetachShader( _programNum, _fragNum );
            }
        glDeleteShader( _fragNum );
        }
    _fragNum = 0;
    }

void surfaceShader::clearVertex()
    {
    if( _vertNum != 0 )
        {
        if( _programNum != 0 )
            {
            glDetachShader( _programNum, _vertNum );
            }
        glDeleteShader( _vertNum );
        }
    _vertNum = 0;
    }

END_JLIB_GRAPHICS_SHADER_NAMESPACE
