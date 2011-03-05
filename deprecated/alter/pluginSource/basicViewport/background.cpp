#include "background.h"

background::background( jLib::graphics::entity::entityG *p ) : triangleCloud( p ), _topColour( "top" ),
        _bottomColour( "bottom" ), _shader( 0 )
    {
    this->addQuad( addPoint( jLib::math::triple( 1, -1, 0 ), 0.0, 0.0, 0.0 ),
                   addPoint( jLib::math::triple( 1, 1, 0 ), 0.0, 0.0, 0.0 ),
                   addPoint( jLib::math::triple( -1, 1, 0 ), 0.0, 0.0, 0.0 ),
                   addPoint( jLib::math::triple( -1, -1, 0 ), 0.0, 0.0, 0.0 ) );
    shaderVariableSet().add( _topColour );
    shaderVariableSet().add( _bottomColour );

    _topColour = jLib::math::colour4( 0.15 );
    _bottomColour = jLib::math::colour4( 0.5 );
    }

background::~background()
    {
    if( _shader )
        {
        delete _shader;
        }
    }

void background::draw()
    {
    if( !_shader )
        {
        refreshShader();
        }
    else
        {
        triangleCloud::draw();
        }
    }

void background::refreshShader()
    {
    _shader = new jLib::graphics::shader::surfaceShader(  "uniform vec4 top;"
                                                    "uniform vec4 bottom;"
                                                    "varying vec4 col;"
                                                    "void main()"
                                                        "{"
                                                        "gl_FragColor = col;"
                                                        "gl_FragDepth = 0.999;"
                                                        "}",
                                                    "uniform vec4 top;"
                                                    "uniform vec4 bottom;"
                                                    "varying vec4 col;"
                                                    "void main()"
                                                        "{"
                                                        "gl_Position = gl_Vertex;"
                                                        "float t = ( ( gl_Vertex.y + 1 ) / 2 );"
                                                        "t = t*2;"
                                                        "col = t * top + ( 1 - t ) * bottom;"
                                                        "}" );
    setShader( *_shader );
    setUseShaderVariableSet( );
    shaderVariableSet().setShader( *_shader );
    shaderVariableSet().refreshLocations();
    }

