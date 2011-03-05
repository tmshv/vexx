#include "graphics/shape/graphics.shape.box.h"

BEGIN_JLIB_GRAPHICS_SHAPE_NAMESPACE

box::box( math::triple s, math::triple pos ) : position( pos ), size( s )
    {
    }

box::~box( )
    {
    }

JFLOAT box::shortestDistance( math::triple )
    {
    return floatLimits::infinity();
    }

JFLOAT box::shortestDistance( line )
    {
    return floatLimits::infinity();
    }

END_JLIB_GRAPHICS_SHAPE_NAMESPACE
