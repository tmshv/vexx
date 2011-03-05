#ifndef GRAPHICS_SHAPE_CUBOID_H
#define GRAPHICS_SHAPE_CUBOID_H

#include "graphics/shape/graphics.shape.types.h"

BEGIN_JLIB_GRAPHICS_SHAPE_NAMESPACE

class JLIB_EXPORT cuboid
    {
    jSpecificProperty( jLib::math::triple, minimum, protected, public )
    jSpecificProperty( jLib::math::triple, maximum, protected, public )
public:
    cuboid();
    cuboid( jLib::math::triple, jLib::math::triple );

    jLib::math::triple size() const;
    JFLOAT volume() const;
    void clear();
    void suggest( const jLib::math::triple & );
    void suggest( const cuboid & );
    bool test( const cuboid & ) const;
    bool test( const jLib::math::triple & ) const;

    static cuboid fromCuboid( const cuboid &, jLib::math::triple, jLib::math::triple );

private:
    bool _clear;
    };

END_JLIB_GRAPHICS_SHAPE_NAMESPACE

#endif
