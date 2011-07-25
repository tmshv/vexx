#ifndef JLIB_GRAPHICS_VIEWPORT_TOOLS_TRANSLATE_H
#define JLIB_GRAPHICS_VIEWPORT_TOOLS_TRANSLATE_H

#include "graphics/viewport/tools/graphics.viewport.tools.types.h"
#include "graphics/viewport/tools/graphics.viewport.tools.base.h"
#include "graphics/implicit/graphics.implicit.vectorAxis.h"

BEGIN_JLIB_GRAPHICS_VIEWPORT_TOOLS_NAMESPACE

class JLIB_EXPORT translate : public base
    {
public:
    translate( );
    virtual entity::entityD *getDrawHandle();
    virtual void highlight( );
    virtual void reset( );
    int activeAxis( );
    math::triple getTranslateDelta();
    static void setQuadSize( JFLOAT );
    static JFLOAT getQuadSize( );
protected:
    virtual void calculateClickLineIntersection( );
private:
    implicit::vectorAxis _axis;
    int _selectedHandle;
    static JFLOAT _quadSize;
    };

END_JLIB_GRAPHICS_VIEWPORT_TOOLS_NAMESPACE

#endif
