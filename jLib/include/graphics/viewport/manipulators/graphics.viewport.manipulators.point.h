#ifndef JLIB_GRAPHICS_VIEWPORT_MANIPULATORS_POINT_H
#define JLIB_GRAPHICS_VIEWPORT_MANIPULATORS_POINT_H
#if 0
#include "graphics/viewport/manipulators/graphics.viewport.manipulators.types.h"
#include "graphics/viewport/manipulators/graphics.viewport.manipulators.base.h"
#include "graphics/viewport/tools/graphics.viewport.tools.translate.h"

BEGIN_JLIB_GRAPHICS_VIEWPORT_MANIPULATORS_NAMESPACE

class JLIB_EXPORT point : public base
    {
public:
    point( manipulatorManager * );
    virtual JFLOAT canHandle( renderPass *, const mouseEvent *, const mouseEvent * );
    virtual int handle( renderPass *, const mouseEvent *, const mouseEvent * );
    };

END_JLIB_GRAPHICS_VIEWPORT_MANIPULATORS_NAMESPACE
#endif
#endif
