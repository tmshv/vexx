#ifndef GRAPHICS_IMPLICIT_SPHERE_H
#define GRAPHICS_IMPLICIT_SPHERE_H

#include "graphics/implicit/graphics.implicit.types.h"
#include "graphics/implicit/graphics.implicit.circle.h"

BEGIN_JLIB_GRAPHICS_IMPLICIT_NAMESPACE

class JLIB_EXPORT lineSphere : public circle
    {
public:
    lineSphere( entity::entityG *p, JFLOAT radius=1, unsigned int divisions=10, math::colour4 colour=math::colour4() );

    virtual void rebuild();
    };

END_JLIB_GRAPHICS_IMPLICIT_NAMESPACE

#endif
