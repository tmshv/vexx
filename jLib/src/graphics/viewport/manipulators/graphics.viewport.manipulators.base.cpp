#include "graphics/viewport/manipulators/graphics.viewport.manipulators.base.h"
#include "graphics/viewport/tools/graphics.viewport.tools.base.h"

BEGIN_JLIB_GRAPHICS_VIEWPORT_MANIPULATORS_NAMESPACE

base::base( ) : _scene( 0 ), manager( 0 ), _currentTool( 0 ), _hasActiveTool( 0 ), _active( FALSE ), _currentSpace( LOCAL ), _toolJustExited( FALSE )
    {
    }

base::~base( )
    {
    }

void base::onActivate()
    {
    jAssert( _scene->camera() );
    _active = TRUE;

    for( unsigned int x=0; x<_toolMembers.size(); x++ )
        {
        entity::entityD *draw = _toolMembers[x].tool->getDrawHandle();

        _toolMembers[x].tool->setScene( _scene );

        if( draw )
            {
            _scene->addChild( draw );
            _scene->camera()->addObserver( this );
            }
        }
    onActivateSub();
    getManager().setUpdate();
    }

void base::onDeactivate()
    {
    onDeactivateSub();

    for( unsigned int x=0; x<_toolMembers.size(); x++ )
        {
        entity::entityD *draw = _toolMembers[x].tool->getDrawHandle();

        _toolMembers[x].tool->setScene( 0 );

        if( draw )
            {
            _scene->removeChild( draw );
            _scene->camera()->removeObserver( this );
            }
        }
    _active = FALSE;
    }

void base::onActivateSub()
    {
    }

void base::onDeactivateSub()
    {
    }

manipulatorManager &base::getManager( )
    {
    jAssert( manager );
    return *manager;
    }

entity::entityG *base::scene( )
    {
    jAssert( _scene );
    return _scene;
    }

bool base::isActive()
    {
    return _active;
    }

void base::setCurrentMouseState( const mouseEvent *curr, const mouseEvent *prev )
    {
    currentMouse = curr;
    previousMouse = prev;
    shape::line currentClickLine = _scene->camera()->screenToVector( currentMouse->mouseX, currentMouse->mouseY );

    if( !_hasActiveTool )
        {
        _currentTool = _toolMembers.size();
        JFLOAT minDistance = floatLimits::infinity();
        for( unsigned int x=0; x<_toolMembers.size(); x++ )
            {
            JFLOAT distance;
            if( _toolMembers[x].tool->isEntryButtonState( curr->buttons, prev->buttons ) )
                {
                _toolMembers[x].tool->setClickLine( currentClickLine );
                _toolMembers[x].tool->calculateClickLineIntersection( );
                if( ( distance = _toolMembers[x].tool->canHandleClickLineAt() ) <= minDistance )
                    {
                    minDistance = distance;
                    _currentTool = x;
                    }
                }
            }
        }
    else
        {
        _toolMembers[_currentTool].tool->setClickLine( currentClickLine );
        if( _toolMembers[_currentTool].tool->isExitButtonState( curr->buttons, prev->buttons ) )
            {
            _hasActiveTool = FALSE;
            _toolJustExited = TRUE;
            }
        }
    }

void base::onCameraUpdate()
    {
    FOREACH( _toolMembers, curTool )
        {
        curTool->tool->setTransform( curTool->tool->getTransform() );
        }
    }

bool base::canHandle( )
    {
    if( _toolJustExited )
        {
        return TRUE;
        }
    return _currentTool < _toolMembers.size();
    }

void base::handle( )
    {
    if( _toolJustExited )
        {
        _toolMembers[_currentTool].tool->reset();
        getManager().setUpdate();
        _currentTool = _toolMembers.size();
        _toolJustExited = FALSE;
        _hasActiveTool = FALSE;
        }
    else
        {
        if( !_hasActiveTool )
            {
            _toolMembers[_currentTool].tool->highlight();
            getManager().setUpdate();
            firstManipulate( _toolMembers[_currentTool].tool );
            }
        _hasActiveTool = TRUE;

        manipulate( _toolMembers[_currentTool].tool );
        }
    }

void base::manipulate( tools::base *)
    {
    }

void base::firstManipulate( tools::base *)
    {
    }

void base::addTool( tools::base &in )
    {
    if( !isToolRegistered( in ) )
        {
        toolPacket temp;
        temp.active = FALSE;
        temp.tool = &in;
        _toolMembers.push_back( temp );
        }
    if( isActive() )
        {
        getManager().setUpdate();
        }
    }

bool base::isToolRegistered( const tools::base &in )
    {
    for( unsigned int x=0; x<_toolMembers.size(); x++ )
        {
        if( _toolMembers[x].tool == &in )
            {
            return TRUE;
            }
        }
    return FALSE;
    }

void base::removeTool( tools::base &in )
    {
    for( unsigned int x=0; x<_toolMembers.size(); x++ )
        {
        if( _toolMembers[x].tool == &in )
            {
            _toolMembers.erase( _toolMembers.begin() + x );
            }
        }
    }

void base::setTransformSpace( spaceType space )
    {
    if( !_hasActiveTool )
        {
        FOREACH( _toolMembers, member )
            {
            member->tool->setTransformSpace( space );
            }
        _currentSpace = space;
        getManager().setUpdate();
        }
    }

base::spaceType base::getTransformSpace( ) const
    {
    return _currentSpace;
    }

END_JLIB_GRAPHICS_VIEWPORT_MANIPULATORS_NAMESPACE
