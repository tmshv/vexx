#include "graphics/shader/graphics.shader.postProcess.h"

#define POSTPROCESSVERTEXSHADER "uniform sampler2D colour;"     \
"uniform sampler2D depth;"                                      \
"void main()"                                                   \
"{"                                                             \
"gl_TexCoord[0].st = 0.5 * ( 1 + gl_Vertex.xy );"               \
"gl_Position = gl_Vertex;"                                      \
"}"

BEGIN_JLIB_GRAPHICS_SHADER_NAMESPACE

postProcess::postProcess( entity::entityG *p, string fS ) : triangleCloud( p ), surfaceShader( fS, POSTPROCESSVERTEXSHADER )/*,
        _colour( colName ), _depth( depName )*/
    {
    setTriangleArraySize( 2 );
    setPointArraySize( 4 );

    addQuad( addPoint( math::triple( 1, 1, 0 ), math::triple( 0, 0, 0 ), math::vector2( 1, 1 ), math::colour4::WHITE ),
        addPoint( math::triple( 1, -1, 0 ), math::triple( 0, 0, 0 ), math::vector2( 1, -1 ), math::colour4::WHITE ),
        addPoint( math::triple( -1, -1, 0 ), math::triple( 0, 0, 0 ), math::vector2( -1, -1 ), math::colour4::WHITE ),
        addPoint( math::triple( -1, 1, 0 ), math::triple( 0, 0, 0 ), math::vector2( -1, 1 ), math::colour4::WHITE ) );

    triangleCloud::setShader( *this );

    //triangleCloud::shaderVariableSet().add( _colour );
    //triangleCloud::shaderVariableSet().add( _depth );
    triangleCloud::shaderVariableSet().setShader( *this );
    triangleCloud::setUseShaderVariableSet( );
    }

void postProcess::setSource( string frag )
    {
    setFragmentSource( frag );
    }

void postProcess::rebuild( )
    {
    surfaceShader::rebuild();
    }

bool postProcess::isValid()
    {
    return surfaceShader::isValid();
    }

unsigned int postProcess::sizeErrors()
    {
    return surfaceShader::sizeErrors();
    }

string postProcess::getError( unsigned int x )
    {
    return surfaceShader::getError( x );
    }

void postProcess::clearErrors()
    {
    surfaceShader::clearErrors();
    }

void postProcess::clearError( unsigned int x )
    {
    surfaceShader::clearError( x);
    }

/*sSampler2D *postProcess::sampler( string name )
    {
    jAssert( dynamic_cast <sSampler2D> ( triangleCloud::shaderVariableSet().find( name ) ) );
    return static_cast <sSampler2D> ( triangleCloud::shaderVariableSet().find( name ) );
    }*/

/*sSampler2D &postProcess::colour()
    {
    return _colour;
    }

sSampler2D &postProcess::depth()
    {
    return _depth;
    }*/

END_JLIB_GRAPHICS_SHADER_NAMESPACE
