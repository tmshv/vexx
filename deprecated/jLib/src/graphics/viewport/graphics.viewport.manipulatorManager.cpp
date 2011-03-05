#include "graphics/viewport/graphics.viewport.manipulatorManager.h"
#include "graphics/viewport/manipulators/graphics.viewport.manipulators.base.h"

BEGIN_JLIB_GRAPHICS_VIEWPORT_NAMESPACE

manipulatorManager::manipulatorManager( ) : dormantColour( 1, 0, 1 ), activeColour( 1, 1, 0 )/*, _points( &_view.pass() )*/,
        _changed( 0 ), _currentActiveManip( 0 )
    {
    //_points.setDrawSize( 4 );
    }

manipulatorManager::manipulatorManager( render::scene &_view ) : dormantColour( 1, 0, 1 ),
        activeColour( 1, 1, 0 )/*, _points( &_view.pass() )*/, _changed( 0 ), _currentActiveManip( 0 )
    {
    _parentView = &_view;
    mouseEvent( 0, 0, 0, 0, 0, 0 );
    //_points.setDrawSize( 4 );
    }

void manipulatorManager::setScene( render::scene &_view )
    {
    _parentView = &_view;
    mouseEvent( 0, 0, 0, 0, 0, 0 );
    }

void manipulatorManager::mouseEvent( unsigned int x, unsigned int y, int but, bool shift, bool ctrl, bool alt )
    {
    oldMouse = currentMouse;

    struct timeval tv;
    gettimeofday(&tv, 0);
    currentMouse.mouseX = x;
    currentMouse.mouseY = y;
    currentMouse.buttons = but;
    currentMouse.modifiers = (((int)shift)|(((int)ctrl)<<1)|(((int)alt)<<2));
    currentMouse.timeSeconds = tv.tv_sec;
    currentMouse.timeMilliseconds = tv.tv_usec;

    distributeEventToHandlers();
    }

void manipulatorManager::keyEvent( int in, bool direction, bool repeat )
    {
    if( _currentActiveManip && in == KBACKSLASH && !repeat && direction )
        {
        manipulators::base::spaceType space = _currentActiveManip->getTransformSpace();
        if( space == manipulators::base::WORLD )
            {
            space = manipulators::base::LOCAL;
            }
        else
            {
            space = manipulators::base::WORLD;
            }
        _currentActiveManip->setTransformSpace( space );
        }
    }

void manipulatorManager::scrollEvent( int )
    {
#warning i should really use this callback
    }

void manipulatorManager::addHandler( manipulators::base *add )
    {
    for( unsigned int x=0; x<handlers.size(); x++ )
        {
        if( add == handlers[x] )
            {
            return;
            }
        }
    handlers.push_back( add );
    }

void manipulatorManager::addEntity( math::triple &pt, math::matrix4x4 mat )
    {
    entityRef temp;
    temp.type = entityRef::point;
    temp.entity = reinterpret_cast <void *> (&pt);
    temp.local = mat;
    temp.selected = 0;
    temp.illustrate = 0;
    _entities.push_back( temp );
    }


/*void manipulatorManager::setActiveTool( tools::base *tool )
    {
    clearActiveTool();
    _activeTool = tool;
    if( _activeTool->getDrawHandle() )
        {
        _parentView->addChild( _activeTool->getDrawHandle() );
        }
    setChanged();
    }

void manipulatorManager::clearActiveTool()
    {
    if( _activeTool != 0 )
        {
        if( _activeTool->getDrawHandle() )
            {
            _parentView->removeChild( _activeTool->getDrawHandle() );
            }
        _activeTool = 0;
        setChanged();
        }
    }*/

void manipulatorManager::removeHandler( manipulators::base *remove )
    {
    for( unsigned int x=0; x<handlers.size(); x++ )
        {
        if( remove == handlers[x] )
            {
            handlers.erase( handlers.begin() + x );
            return;
            }
        }
    }

bool manipulatorManager::needsUpdate()
    {
    return _update;
    }

void manipulatorManager::updated()
    {
    _update = FALSE;
    }

vector <manipulatorManager::entityRef *> manipulatorManager::getEntitys( entityRef::entityType type )
    {
    vector <entityRef *> ret;

    for( unsigned int x=0; x<_entities.size(); x++ )
        {
        if( _entities[x].type == type )
            {
            ret.push_back( &(_entities[x]) );
            }
        }

    return ret;
    }

vector <manipulatorManager::entityRef *> manipulatorManager::getSelectedEntitys( entityRef::entityType type )
    {
    vector <entityRef *> ret;

    for( unsigned int x=0; x<_entities.size(); x++ )
        {
        if( _entities[x].type == type && _entities[x].selected )
            {
            ret.push_back( &(_entities[x]) );
            }
        }

    return ret;
    }

manipulatorManager::entityRef *manipulatorManager::getEntity( math::triple &in )
    {
    for( unsigned int x=0; x<_entities.size(); x++ )
        {
        if( _entities[x].entity == &in )
            {
            return &(_entities[x]);
            }
        }
    return 0;
    }

/*void manipulatorManager::handleCurrentTool()
    {
    int ret = _activeTool->handle( _parentView, &currentMouse, &oldMouse );
    if( (ret&&manipulatorManager::CONTINUE) == 0 )
        {
        clearActiveTool();
        }
    if( (ret&&manipulatorManager::UPDATE) != 0 )
        {
        _update = TRUE;
        }
    }
*/
void manipulatorManager::handleCurrentManip()
    {
    if( _currentActiveManip )
        {
        _currentActiveManip->setCurrentMouseState( &currentMouse, &oldMouse );
        if( _currentActiveManip->canHandle() )
            {
            _currentActiveManip->handle();
            }
        else
            {
            deactivate();
            }
        }
    /*int ret = _currentActiveManip->tryHandle( _parentView, &currentMouse, &oldMouse );
    if( (ret&(manipulatorManager::ACTIVE|manipulatorManager::PASSIVE)) == 0 )
        {
        _currentActiveManip = 0;
        }
    if( (ret&&manipulatorManager::UPDATE) != 0 )
        {
        _update = TRUE;
        }*/
    }

void manipulatorManager::distributeEventToHandlers()
    {
    static bool motion = FALSE;
    jAssert( _parentView );
    if( ( currentMouse.modifiers & MALT ) != FALSE && ( oldMouse.buttons == FALSE || motion ) )
        {
        if( currentMouse.buttons == LEFT )
            {
            _parentView->camera().motion( camera::base::ROTATE, (float)currentMouse.mouseX / _parentView->width(), (float)currentMouse.mouseY / _parentView->height() );
            setUpdate();
            motion = TRUE;
            }
        else if( currentMouse.buttons == MIDDLE ||
                ( ( currentMouse.buttons & LEFT ) && ( currentMouse.buttons & RIGHT ) ) != FALSE )
            {
            _parentView->camera().motion( camera::base::TRANSLATE, (float)currentMouse.mouseX / _parentView->width(), (float)currentMouse.mouseY / _parentView->height() );
            setUpdate();
            motion = TRUE;
            }
        else if( currentMouse.buttons == RIGHT )
            {
            _parentView->camera().motion( camera::base::ZOOM, (float)currentMouse.mouseX / _parentView->width(), (float)currentMouse.mouseY / _parentView->height() );
            setUpdate();
            motion = TRUE;
            }
        else
            {
            _parentView->camera().motion( camera::base::NONE, 0, 0 );
            setUpdate();
            motion = FALSE;
            }
        }
    else
        {
        if( motion )
            {
            _parentView->camera().motion( camera::base::NONE, 0, 0 );
            setUpdate();
            motion = FALSE;
            }
        if( _currentActiveManip != 0 )
            {
            handleCurrentManip();
            }
        }

    /*if( _currentActiveManip == 0 )
        {
        float handlerMax = floatLimits::infinity();
        for( unsigned int x=0; x<handlers.size(); x++ )
            {
            if( handlers[x]->isEnabled() )
                {
                /float tempMax = handlers[x]->canHandle( _parentView, &currentMouse, &oldMouse );
                if( tempMax != floatLimits::nan() && ( tempMax < handlerMax || ( handlerMax == floatLimits::infinity() && tempMax == floatLimits::infinity() ) ) )
                    {
                    handlerMax = tempMax;
                    _currentActiveManip = handlers[x];
                    handleCurrentManip();
                    }
                }
            }
        }*/
    }

void manipulatorManager::deselectAll()
    {
    for( unsigned int x=0; x<_entities.size(); x++ )
        {
        _entities[x].selected = FALSE;
        }
    }

void manipulatorManager::rebuildDrawLayer()
    {
    // Rebuild point types
    /*vector <entityRef *> ptData = getEntitys( entityRef::point );
    _points.clear();
    _points.setPointArraySize( ptData.size() );
    for( unsigned int x=0; x<ptData.size(); x++ )
        {
        math::colour4 ptCol = dormantColour;
        if( ptData[x]->selected )
            {
            ptCol = activeColour;
            }
        _points.addPoint( ptData[x]->local.pointMultiply( *(reinterpret_cast <math::triple *> (ptData[x]->entity) ) ), ptCol );
        }
    setChanged();*/
    }

void manipulatorManager::setUpdate( bool able )
    {
    _update = able;
    }

bool manipulatorManager::getChanged()
    {
    return _changed;
    }

void manipulatorManager::setChanged( bool able )
    {
    _changed = able;
    }

void manipulatorManager::activate( manipulators::base &manip )
    {
    _currentActiveManip = &manip;
    _currentActiveManip->_scene = &(_parentView->pass());
    _currentActiveManip->manager = this;
    _currentActiveManip->onActivate();
    }

void manipulatorManager::deactivate( )
    {
    _currentActiveManip->onDeactivate();
    _currentActiveManip->_scene = 0;
    _currentActiveManip->manager = 0;
    _currentActiveManip = 0;
    }

END_JLIB_GRAPHICS_VIEWPORT_NAMESPACE
