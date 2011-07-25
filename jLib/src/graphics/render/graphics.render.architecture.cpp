#include "graphics/render/graphics.render.architecture.h"
#include "jLib.glHeader.h"

BEGIN_JLIB_GRAPHICS_RENDER_NAMESPACE

architecture::architecture( )
    {
    root = 0;
    _colFormat = frameBuffer::COLOUR_32BIT_INT;
    _depFormat = frameBuffer::DEPTH_8BIT_INT;
    }

void architecture::setFrameBufferFormat( frameBuffer::colourFormat col, frameBuffer::depthFormat dep )
    {
    _colFormat = col;
    _depFormat = dep;
    }

architecture::~architecture( )
    {
    empty();
    }

void architecture::addScene( string inName, scene &inScene )
    {
    sceneRef *temp = new sceneRef;

    temp->name = inName;
    temp->sce = &inScene;
    temp->post = 0;
    temp->buf = 0;
    temp->buf = new jLib::graphics::render::frameBuffer( frameBuffer::COLOUR|frameBuffer::DEPTH, _colFormat, _depFormat );

    temp->sce->setRenderTarget( *temp->buf );

    scenes.push_back( temp );
    }

void architecture::addPostProcess( string inName, string source )
    {
    sceneRef *temp = new sceneRef;

    temp->name = inName;
    temp->sce = new scene( math::colour4::BLACK );
    temp->post = new shader::postProcess( temp->sce->passP(), source );
    temp->buf = new jLib::graphics::render::frameBuffer( frameBuffer::COLOUR|frameBuffer::DEPTH, _colFormat, _depFormat );

    temp->sce->setRenderTarget( *temp->buf );

    scenes.push_back( temp );
    }

pixel::texture &architecture::texture( string name, bufferType bufName )
    {
    sceneRef *scene = findScene( name );
    jAssert( scene != root );
    jAssert( scene->buf );

    return scene->buf->texture( (frameBuffer::mode)bufName );
    }

void architecture::setRenderable( string name )
    {
    jRenderDebug( "Set Renderable scene - "<<name );
    if( root )
        {
        root->buf = new jLib::graphics::render::frameBuffer( frameBuffer::COLOUR|frameBuffer::DEPTH, _colFormat, _depFormat );
        }

    root = findScene( name );
    jAssert( root );

    if( root->buf )
        {
        root->sce->clearRenderTarget();
        delete root->buf;
        root->buf = 0;
        }
    }

void architecture::addPostProcessInput( string postName, string outputName, string varName, bufferType bufName )
    {
    jRenderDebug( "Add post process input to "<<postName<<", coming from input "<<outputName );
    sceneRef *postScene = findScene( postName );
    sceneRef *outputScene = findScene( outputName );
    jAssert( postScene );
    jAssert( postScene->post );
    jAssert( outputScene );
    jAssert( outputScene->buf );
    jAssert( outputScene != root );

    sceneDependancy temp;

    temp.type = bufName;
    temp.sce = outputScene;

    temp.sampler = new shader::sSampler2D( varName );
    jRenderDebug( "Construct sampler '"<<varName<<"', "<<temp.sampler );

    postScene->post->shaderVariableSet().add( *(temp.sampler) );

    postScene->post->shaderVariableSet().refreshLocations();

    temp.sampler->set( &(outputScene->buf->texture( (frameBuffer::mode)bufName )) );

    postScene->dep.push_back( temp );
    }

void architecture::render()
    {
    jAssert( root );
    FOREACH( scenes, sce )
        {
        (*sce)->dirty = TRUE;
        }

    ensureDependancies( root );
    render( root );
    }

void architecture::ensureDependancies( sceneRef *scene )
    {
    if( !scene->dirty )
        {
        return;
        }
    jRenderDebug( "Ensure dependancies for "<<scene );
    FOREACH( scene->dep, dependancy )
        {
        if( dependancy->sce->dirty )
            {
            jRenderDebug( "Dirty scene found, Ensure dependancies for "<<dependancy->sce );
            ensureDependancies( dependancy->sce );

            jRenderDebug( "Render Scene"<<dependancy->sce );
            render( dependancy->sce );
            }
        }
    scene->dirty = FALSE;
    }

void architecture::render( sceneRef *scene )
    {
    jAssert( !scene->dirty );
    unsigned int texUnit = 0;

    jRenderDebug( "Pre Render Scene "<<scene->name );
    FOREACH( scene->dep, dependancy )
        {
        jRenderDebug( "Bind Input for scene "<<&(dependancy->sce->buf->texture( (frameBuffer::mode)dependancy->type ) )<<" to unit "<<texUnit );
        dependancy->sce->buf->texture( (frameBuffer::mode)dependancy->type ).setTextureUnit( texUnit );
        dependancy->sce->buf->texture( (frameBuffer::mode)dependancy->type ).bind();

        ++texUnit;
        }

    jRenderDebug( "Render Scene "<<scene );
    scene->sce->render();

    FOREACH( scene->dep, dependancy )
        {
        jRenderDebug( "Unbind Input "<<&(dependancy->sce->buf->texture( (frameBuffer::mode)dependancy->type ) ) );
        dependancy->sce->buf->texture( (frameBuffer::mode)dependancy->type ).unbind();
        }
    }

void architecture::setSize( unsigned int w, unsigned int h )
    {
    jRenderDebug( "Set Size"<<w<<" "<<h );
    FOREACH( scenes, sce )
        {
        (*sce)->sce->setSize( w, h );
        }
    }

void architecture::updateContext()
    {
    jRenderDebug( "Update Context" );
    glewInit();
    glEnable( GL_DEPTH_TEST );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    }

void architecture::empty()
    {
    while( scenes.size() )
        {
        FOREACH( scenes.front()->dep, d )
            {
            jRenderDebug( "Delete scene dependancy" );
            delete d->sampler;
            }
        if( scenes.front()->post )
            {
            delete scenes.front()->sce;
            delete scenes.front()->post;
            }
        if( scenes.front()->buf )
            {
            delete scenes.front()->buf;
            }
        delete scenes.front();
        scenes.erase( scenes.begin() );
        }
    }

architecture::sceneRef *architecture::findScene( string name )
    {
    FOREACH( scenes, sce )
        {
        if( (*sce)->name == name )
            {
            return *sce;
            }
        }
    return 0;
    }

END_JLIB_GRAPHICS_RENDER_NAMESPACE
