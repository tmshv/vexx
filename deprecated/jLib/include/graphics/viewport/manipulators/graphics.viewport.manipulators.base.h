#ifndef JLIB_GRAPHICS_VIEWPORT_MANIPULATORS_BASE_H
#define JLIB_GRAPHICS_VIEWPORT_MANIPULATORS_BASE_H

#include "graphics/viewport/manipulators/graphics.viewport.manipulators.types.h"
#include "graphics/viewport/tools/graphics.viewport.tools.types.h"
#include "graphics/viewport/graphics.viewport.manipulatorManager.h"
#include "graphics/camera/graphics.camera.base.h"

BEGIN_JLIB_GRAPHICS_VIEWPORT_MANIPULATORS_NAMESPACE

class JLIB_EXPORT base : public graphics::camera::cameraObserver
    {
public:
    enum spaceType { LOCAL = 0, WORLD = 1 };
    base( );
    virtual ~base( );
    void setCurrentMouseState( const mouseEvent *, const mouseEvent * );
    virtual void onCameraUpdate();
    bool canHandle( );
    void handle( );
    void onActivate();
    void onDeactivate();
    void setTransformSpace( spaceType=LOCAL );
    spaceType getTransformSpace( ) const;
protected:
    void addTool( tools::base & );
    bool isToolRegistered( const tools::base & );
    void removeTool( tools::base & );
    const mouseEvent *currentMouse;
    const mouseEvent *previousMouse;
    manipulatorManager &getManager( );
    entity::entityG *scene( );
    bool isActive( );
    virtual void manipulate( tools::base * );
    virtual void firstManipulate( tools::base * );
private:
    virtual void onActivateSub();
    virtual void onDeactivateSub();
    struct toolPacket
        {
        bool active;
        tools::base *tool;
        };
    vector <toolPacket> _toolMembers;
    entity::entityG *_scene;
    manipulatorManager *manager;
    unsigned int _currentTool;
    bool _hasActiveTool;
    bool _active;
    spaceType _currentSpace;
    bool _toolJustExited;
    friend class viewport::manipulatorManager;
    };

END_JLIB_GRAPHICS_VIEWPORT_MANIPULATORS_NAMESPACE

#endif
