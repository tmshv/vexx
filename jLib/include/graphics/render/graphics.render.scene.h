#ifndef JLIB_GRAPHICS_VIEWPORT_RENDERABLESCENE_H
#define JLIB_GRAPHICS_VIEWPORT_RENDERABLESCENE_H

#include "graphics/render/graphics.render.types.h"
#include "graphics/render/graphics.render.target.h"
#include "graphics/camera/graphics.camera.base.h"
#include "graphics/camera/graphics.camera.perspective.h"
#include "graphics/entity/graphics.entity.h"

BEGIN_JLIB_GRAPHICS_RENDER_NAMESPACE

class JLIB_EXPORT scene
    {
    jROProperty( unsigned int, width )
    jROProperty( unsigned int, height )
public:
    scene( math::colour4=math::colour4(), unsigned int=1, unsigned int=1 );
    ~scene( );

    void setRenderTarget( render::target & );
    void clearRenderTarget( );

    void addScene( entity::entityG * );
    void removeScene( entity::entityG * );
    void removeScene( unsigned int );
    void setPassCamera( unsigned int, camera::base & );
    entity::entityG &pass( unsigned int = 0 );
    entity::entityG *passP( unsigned int = 0 );

    camera::base &defaultCamera();
    void addCamera( camera::base & );
    void removeCamera( unsigned int );
    void removeCamera( camera::base & );
    camera::base &camera( unsigned int = 0 );

    math::colour4 bgColour();
    //unsigned int width();
    //unsigned int height();
    JFLOAT ratio();
    void setSize( unsigned int, unsigned int );
    void setBgColour( math::colour4 );

    void render();
private:
    /*struct sceneRef
        {
        entity::entityG *scene;
        camera::base *camera;
        };*/
    render::target *_target;
    vector <camera::base *> _cameras;
    entity::entityG _defaultScene;
    camera::perspective _defaultCamera;
    vector <entity::entityG *> _passes;
    math::colour4 _bgCol;
    };

END_JLIB_GRAPHICS_RENDER_NAMESPACE

#endif
