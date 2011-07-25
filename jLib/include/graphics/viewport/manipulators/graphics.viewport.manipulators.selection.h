#ifndef JLIB_GRAPHICS_VIEWPORT_MANIPULATORS_SELECTION_H
#define JLIB_GRAPHICS_VIEWPORT_MANIPULATORS_SELECTION_H
#if 0
#include "graphics/viewport/manipulators/graphics.viewport.manipulators.types.h"
#include "graphics/viewport/manipulators/graphics.viewport.manipulators.base.h"

BEGIN_JLIB_GRAPHICS_VIEWPORT_MANIPULATORS_NAMESPACE

class JLIB_EXPORT selection : public base
    {
public:
    selection( manipulatorManager * );
    virtual JFLOAT canHandle( renderPass *, const mouseEvent *, const mouseEvent * );
    virtual int handle( renderPass *, const mouseEvent *, const mouseEvent * );
    void setDistance( JFLOAT );
    JFLOAT getDistance();
private:
    JFLOAT _distance;
    manipulatorManager::entityRef *_select;
    bool _justSelected;
    };

END_JLIB_GRAPHICS_VIEWPORT_MANIPULATORS_NAMESPACE
#endif
#endif
