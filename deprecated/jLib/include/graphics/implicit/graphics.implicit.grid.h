#ifndef JLIB_GRAPHICS_IMPLICIT_GRID_H
#define JLIB_GRAPHICS_IMPLICIT_GRID_H

#include "graphics/implicit/graphics.implicit.types.h"
#include "graphics/primitives/graphics.primitives.lineCloud.h"

BEGIN_JLIB_GRAPHICS_IMPLICIT_NAMESPACE

class JLIB_EXPORT grid : public primitives::lineCloud
    {
public:
    grid( entity::entityG *p, math::colour4 colour=math::colour4(), unsigned int xRes=10, unsigned int yRes=0, JFLOAT xSize=1, JFLOAT ySize=0 );

    void rebuild();
    void setSize( JFLOAT );
    void setXSize( JFLOAT );
    void setYSize( JFLOAT );
    void setRes( unsigned int );
    void setXRes( unsigned int );
    void setYRes( unsigned int );
    void setColour( math::colour4 );
    JFLOAT getXSize( );
    JFLOAT getYSize( );
    unsigned int getXRes( );
    unsigned int getYRes( );
    math::colour4 setColour( );

private:
    math::colour4 _colour;
    unsigned int _xRes;
    unsigned int _yRes;
    JFLOAT _xSize;
    JFLOAT _ySize;
    };

END_JLIB_GRAPHICS_IMPLICIT_NAMESPACE

#endif // GRID_H
