#include "graphics/viewport/manipulators/graphics.viewport.manipulators.point.h"
#if 0
BEGIN_JLIB_GRAPHICS_VIEWPORT_MANIPULATORS_NAMESPACE

point::point( manipulatorManager *p ) : base( p )
    {
    }

JFLOAT point::canHandle( renderPass *vP, const mouseEvent *now, const mouseEvent *prev )
    {
    if( now->buttons & manipulatorManager::RIGHT && prev->buttons == FALSE && now->modifiers == FALSE )
        {
        vector <manipulatorManager::entityRef *> entities = getParent()->getEntitys( manipulatorManager::entityRef::point );
        if( entities.size() > 0 )
            {
            math::triple ret;
            for( unsigned int x=0; x<entities.size(); x++ )
                {
                ret += *entities[x]->getAs<math::triple>();
                }
            ret /= entities.size();
            return ( vP->getCamera()->translate - ret ).length();
            }
        }
    return floatLimits::nan();
    }

int point::handle( renderPass *, const mouseEvent *, const mouseEvent * )
    {
    //trans.setControlled( getParent()->getSelectedEntitys( manipulatorManager::entityRef::point ) );
    //getParent()->setActiveTool( &trans );
    return 0;
    }

END_JLIB_GRAPHICS_VIEWPORT_MANIPULATORS_NAMESPACE
#endif
