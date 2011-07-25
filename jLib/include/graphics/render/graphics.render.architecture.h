#ifndef JLIB_GRAPHICS_VIEWPORT_ARCHITECTURE_H
#define JLIB_GRAPHICS_VIEWPORT_ARCHITECTURE_H

#include "graphics/render/graphics.render.types.h"
#include "graphics/render/graphics.render.frameBuffer.h"
#include "graphics/render/graphics.render.scene.h"
#include "graphics/shader/graphics.shader.postProcess.h"

BEGIN_JLIB_GRAPHICS_RENDER_NAMESPACE

class JLIB_EXPORT architecture
    {
public:
    enum bufferType { COLOUR=frameBuffer::COLOUR, DEPTH=frameBuffer::DEPTH };
    architecture( );
    ~architecture( );

    void setFrameBufferFormat( frameBuffer::colourFormat, frameBuffer::depthFormat );

    void addScene( string, scene & );
    void addPostProcess( string, string );

    pixel::texture &texture( string, bufferType );

    void setRenderable( string );

    void addPostProcessInput( string, string, string, bufferType );

    void render();
    void setSize( unsigned int, unsigned int );
    void updateContext();
    void empty();

private:
    struct sceneRef;
    struct sceneDependancy
        {
        //sceneDependancy( );// : sampler( new shader::sSampler2D( name ) ) { }
        //~sceneDependancy() { jRenderDebug("Delete Scenedependancy"); delete sampler; }
        bufferType type;
        sceneRef *sce;
        shader::sSampler2D *sampler;
        };
    struct sceneRef
        {
        string name;
        scene *sce;
        shader::postProcess *post;
        frameBuffer *buf;
        vector <sceneDependancy> dep;
        bool dirty;
        };
    vector <sceneRef *> scenes;
    sceneRef *root;
    sceneRef *findScene( string );
    void ensureDependancies( sceneRef * );
    void render( sceneRef * );
    frameBuffer::colourFormat _colFormat;
    frameBuffer::depthFormat _depFormat;
    };

END_JLIB_GRAPHICS_RENDER_NAMESPACE

#endif
