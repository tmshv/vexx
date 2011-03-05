#ifndef JLIB_GRAPHICS_ENTITY_BBOX_H
#define JLIB_GRAPHICS_ENTITY_BBOX_H

#include "graphics/entity/graphics.entity.types.h"
#include "graphics/entity/graphics.entity.entityD.h"
#include "graphics/shape/graphics.shape.line.h"
#include "graphics/shape/graphics.shape.box.h"

BEGIN_JLIB_GRAPHICS_ENTITY_NAMESPACE

class bBox : private shape::box
    {
public:
    //! Construct default empty bbox with a parent *entityD* ( not G )
    bBox( );
    //! Empty the bBox data, making it infinitely small, so its size can be re specified etc.
    void clear();
    //! Suggest a point as a max / min to the bBox
    void suggest( math::triple );
    //! Test whether a point is inside the bBox
    bool isInside( math::triple );
    //! Test whether a line intersects with the box
    bool isInside( shape::line );
    //! Test whether another bBox intersects witht his one
    bool isInside( const bBox & );
    };
END_JLIB_GRAPHICS_ENTITY_NAMESPACE

#endif
