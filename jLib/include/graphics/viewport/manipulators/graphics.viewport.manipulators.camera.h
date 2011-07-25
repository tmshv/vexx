#ifndef JLIB_GRAPHICS_VIEWPORT_MANIPULATORS_CAMERA_H
#define JLIB_GRAPHICS_VIEWPORT_MANIPULATORS_CAMERA_H
#if 0
#include "graphics/viewport/manipulators/graphics.viewport.manipulators.types.h"
#include "graphics/viewport/manipulators/graphics.viewport.manipulators.base.h"

BEGIN_JLIB_GRAPHICS_VIEWPORT_MANIPULATORS_NAMESPACE

class JLIB_EXPORT camera : public base
    {
public:
    camera( manipulatorManager * );
    virtual JFLOAT canHandle( renderPass *, const mouseEvent *, const mouseEvent * );
    virtual int handle( renderPass *, const mouseEvent *, const mouseEvent * );
    };

END_JLIB_GRAPHICS_VIEWPORT_MANIPULATORS_NAMESPACE
#endif
#endif
