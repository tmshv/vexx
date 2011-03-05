#include "graphics/shader/graphics.shader.variableSet.h"
#include "jLib.glHeader.h"

BEGIN_JLIB_GRAPHICS_SHADER_NAMESPACE

void variableSet::setShader( surfaceShader &in )
    {
    jAssert( in.isValid() );
    _shader = &in;
    refreshLocations();
    }

void variableSet::setValues()
    {
    //glUseProgram( _shader->_programNum );
    FOREACH( vars, var )
        {
        jRenderDebug( "Set shader variable "<<(*var) );
        (*var)->set();
        }
    //glUseProgram( 0 );
    }

void variableSet::unsetValues()
    {
    //glUseProgram( _shader->_programNum );
    FOREACH( vars, var )
        {
        jRenderDebug( "Set shader variable "<<(*var) );
        (*var)->unset();
        }
    //glUseProgram( 0 );
    }

bool variableSet::add( surfaceShader::variable &cpy )
    {
    jRenderDebug( "Add variable "<<cpy.name()<<", "<<&cpy<<" ( "<<typeid(cpy).name()<<" )"<<endl<<"\tto shader variable set "<<this );
    FOREACH( vars, var )
        {
        if( (*var)->name() == cpy.name() || &cpy == *var )
            {
            jRenderDebug( "\tFail to add variable, already in set" );
            return false;
            }
        }
    vars.push_back( &cpy );
    jRenderDebug( "\tSuccessfully added variable. there are now "<<vars.size()<<" variables" );
    return true;
    }

void variableSet::clear( )
    {
    vars.clear();
    }

bool variableSet::remove( string name )
    {
    FOREACH( vars, var )
        {
        if( (*var)->name() == name )
            {
            vars.erase( var );
            return true;
            }
        }
    return false;
    }

void variableSet::refreshLocations( )
    {
    FOREACH( vars, var )
        {
        (*var)->refreshLocation( _shader->_programNum ) GLE;
        }
    }

END_JLIB_GRAPHICS_SHADER_NAMESPACE
