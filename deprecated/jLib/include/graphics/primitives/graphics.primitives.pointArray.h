#ifndef JLIB_GRAPHICS_PRIMITIVES_POINTARRAY_H
#define JLIB_GRAPHICS_PRIMITIVES_POINTARRAY_H

#include "graphics/primitives/graphics.primitives.types.h"
#include "graphics/entity/graphics.entity.h"

#ifndef POINTARRAY_NEEDS_COLOUR
#   define POINTARRAY_NEEDS_COLOUR 1
#endif

BEGIN_JLIB_GRAPHICS_PRIMITIVES_NAMESPACE

struct pointArrayPrim
    {
    math::quickPt3 position;
#if POINTARRAY_NEEDS_COLOUR
    math::quickPt4 colour;
#endif
    };
class JLIB_EXPORT pointArray : public entity::entityD
    {
public:
#if !POINTARRAY_NEEDS_COLOUR
    colour4 colour;
#endif
    bool alphaBlend;

    pointArray( entity::entityG *, float s=1 );
    virtual ~pointArray( );

    virtual void removePoint( unsigned int );
    void setPoint( unsigned int, math::triple, math::colour4 );
    void setPointPosition( unsigned int, math::triple );
    void setPointColour( unsigned int, math::colour4 );
    math::triple getPointPosition( unsigned int );
    math::colour4 getPointColour( unsigned int);
    void setPointArraySize( unsigned int );
    unsigned int sizePoints( );
    virtual void clear();
    virtual void clearPoints();
    void setDrawSize( JFLOAT );
    virtual void draw( );

protected:
    float _size;
    vector <struct pointArrayPrim> _pointData;
    };

END_JLIB_GRAPHICS_PRIMITIVES_NAMESPACE

#endif
