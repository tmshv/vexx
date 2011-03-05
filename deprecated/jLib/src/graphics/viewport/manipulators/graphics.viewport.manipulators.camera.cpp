#include "graphics/viewport/manipulators/graphics.viewport.manipulators.camera.h"
#if 0
BEGIN_JLIB_GRAPHICS_VIEWPORT_MANIPULATORS_NAMESPACE

camera::camera( manipulatorManager *controller ) : base( controller )
    {
    }

JFLOAT camera::canHandle( renderPass *, const mouseEvent *now, const mouseEvent *prev )
    {
    if( ((now->modifiers&manipulatorManager::ALT)!=FALSE) && ((now->buttons!=FALSE) && (prev->buttons)==FALSE) && prev->mouseX != now->mouseX && prev->mouseY != now->mouseY )
        {
        return floatLimits::infinity();
        }
    return floatLimits::nan();
    }

int camera::handle( renderPass *vP, const mouseEvent *now, const mouseEvent * )
    {
    if(now->buttons==FALSE)
        {
        vP->getCamera()->motion( jLib::graphics::camera::base::NONE, now->mouseX, now->mouseY );
        return 0;
        }
    else if( now->buttons == manipulatorManager::LEFT )
        {
        vP->getCamera()->motion( jLib::graphics::camera::base::ROTATE, (float)now->mouseX/vP->getWidth(), (float)now->mouseY/vP->getHeight() );
        }
    else if( now->buttons == manipulatorManager::MIDDLE )
        {
        vP->getCamera()->motion( jLib::graphics::camera::base::TRANSLATE, (float)now->mouseX/vP->getWidth(), (float)now->mouseY/vP->getHeight() );
        }
    else if( now->buttons == manipulatorManager::RIGHT )
        {
        vP->getCamera()->motion( jLib::graphics::camera::base::ZOOM, (float)now->mouseX/vP->getWidth(), (float)now->mouseY/vP->getHeight() );
        }

    return manipulatorManager::PASSIVE || manipulatorManager::UPDATE;
    }

END_JLIB_GRAPHICS_VIEWPORT_MANIPULATORS_NAMESPACE
#endif
