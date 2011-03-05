#ifndef JLIB_GRAPHICS_VIEWPORT_MANIPULATORS_TRANSLATE_H
#define JLIB_GRAPHICS_VIEWPORT_MANIPULATORS_TRANSLATE_H

#include "graphics/viewport/manipulators/graphics.viewport.manipulators.types.h"
#include "graphics/viewport/manipulators/graphics.viewport.manipulators.base.h"
#include "graphics/viewport/tools/graphics.viewport.tools.translate.h"
#include "graphics/implicit/graphics.implicit.vectorAxis.h"
#include "graphics/camera/graphics.camera.base.h"

BEGIN_JLIB_GRAPHICS_VIEWPORT_MANIPULATORS_NAMESPACE

class JLIB_EXPORT translate : public base
    {
public:
    translate( );
    //virtual JFLOAT canHandle( renderPass *, const mouseEvent *, const mouseEvent * );
    //virtual int handle( renderPass *, const mouseEvent *, const mouseEvent * );
    void canHandle();
    virtual void manipulate( tools::base * );
    virtual void firstManipulate( tools::base * );
    math::triple currentPosition( );
private:
    //math::triple getNewPos( shape::line );
    vector <manipulatorManager::entityRef *> workingSet;
    virtual void onActivateSub();
    tools::translate tool;
    shape::line prevClickLine;
    math::triple prevPos;
    };

END_JLIB_GRAPHICS_VIEWPORT_MANIPULATORS_NAMESPACE

#endif
