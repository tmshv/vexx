#include "graphics/viewport/manipulators/graphics.viewport.manipulators.selection.h"
#if 0
BEGIN_JLIB_GRAPHICS_VIEWPORT_MANIPULATORS_NAMESPACE

selection::selection( manipulatorManager *controller ) : base( controller ), _distance( 0.2 ),
        _select( 0 ), _justSelected( FALSE )
    {
    }

JFLOAT selection::canHandle( renderPass *vP, const mouseEvent *now, const mouseEvent *prev )
    {
    if( _justSelected == FALSE )
        {
        if( now->buttons == FALSE && prev->buttons & manipulatorManager::LEFT && now->modifiers == FALSE && prev->modifiers == FALSE )
            {
            _select = 0;
            return floatLimits::infinity();
            }
        if( now->buttons & manipulatorManager::LEFT && prev->buttons == FALSE )
            {
            vector <manipulatorManager::entityRef *> entities = getParent()->getEntitys( manipulatorManager::entityRef::point );
            shape::line click = vP->getCamera()->screenToVector( now->mouseX, now->mouseY );

            float closest = floatLimits::infinity();
            unsigned int id = entities.size();
            for( unsigned int x=0; x<entities.size(); x++ )
                {
                float thisDist = click.shortestDistance( math::pointToWorldSpace( *(entities[x]->getAs<math::triple>()), entities[x]->local ) );
                if( thisDist < closest )
                    {
                    closest = thisDist;
                    id = x;
                    }
                }

            if( closest < _distance && id != entities.size() )
                {
                _select = entities[id];
                return ( vP->getCamera()->translate - (*entities[id]->getAs<math::triple>()) ).length();
                }
            }
        }
    else if( now->buttons != prev->buttons )
        {
        return 0;
        }
    return floatLimits::nan();
    }

void selection::setDistance( JFLOAT distance )
    {
    _distance = distance;
    }

JFLOAT selection::getDistance()
    {
    return _distance;
    }

int selection::handle( renderPass *, const mouseEvent *, const mouseEvent * )
    {
    if( _justSelected == TRUE )
        {
        _justSelected = FALSE;
        return 0;
        }
    getParent()->deselectAll();
    //jDebugNeat<<"process select"<<endl;
    if( _select != 0 )
        {
        //jDebugNeat<<"actially select"<<endl;
        _select->selected = TRUE;
        _select = 0;
        _justSelected = TRUE;
        }
    getParent()->rebuildDrawLayer();
    return 0;
    }

END_JLIB_GRAPHICS_VIEWPORT_MANIPULATORS_NAMESPACE
#endif
