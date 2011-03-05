#include "graphics/implicit/graphics.implicit.grid.h"

BEGIN_JLIB_GRAPHICS_IMPLICIT_NAMESPACE

grid::grid( entity::entityG *p, math::colour4 colour, unsigned int xRes, unsigned int yRes, JFLOAT xSize, JFLOAT ySize ) : lineCloud( p )
    {
    if( yRes == 0 )
        {
        yRes = xRes;
        }
    if( ySize == 0 )
        {
        ySize = xSize;
        }
    _xSize = xSize;
    _ySize = ySize;
    _xRes = xRes;
    _yRes = yRes;
    _colour = colour;
    rebuild();
    }

void grid::rebuild()
    {
    unsigned int newXRes = _xRes + 1;
    unsigned int newYRes = _yRes + 1;
    float halfWidth = _xRes * _xSize * 0.5;
    float halfHeight = _yRes * _ySize * 0.5;
    unsigned int corners[4];
    unsigned int edge1, edge2;

    clear();
    setPointArraySize( ( 2 * newXRes ) + ( 2 * newYRes ) - 4 );
    setLineArraySize( newXRes + newYRes );

    for( unsigned int x=1; x<( newXRes-1 ); x++ )
        {
        edge1 = addPoint( math::triple( -halfWidth + ( x * _xSize ), 0, -halfHeight ), _colour );
        edge2 = addPoint( math::triple( -halfWidth + ( x * _xSize ), 0, halfHeight ), _colour );

        addLine( edge1, edge2 );
        }
    for( unsigned int x=1; x<( newYRes-1 ); x++ )
        {
        edge1 = addPoint( math::triple( -halfWidth, 0, -halfHeight + ( x * _ySize ) ), _colour );
        edge2 = addPoint( math::triple( halfWidth, 0, -halfHeight + ( x * _ySize ) ), _colour );

        addLine( edge1, edge2 );
        }

    corners[0] = addPoint( math::triple( halfWidth, 0, -halfHeight ), _colour );
    corners[1] = addPoint( math::triple( halfWidth, 0, halfHeight ), _colour );
    corners[2] = addPoint( math::triple( -halfWidth, 0, halfHeight ), _colour );
    corners[3] = addPoint( math::triple( -halfWidth, 0, -halfHeight ), _colour );

    addLineLoop( corners, 4 );
    }

void grid::setSize( JFLOAT s )
    {
    setXSize( s );
    setYSize( s );
    rebuild();
    }

void grid::setXSize( JFLOAT s )
    {
    _xSize = s;
    rebuild();
    }

void grid::setYSize( JFLOAT s )
    {
    _ySize = s;
    rebuild();
    }

void grid::setRes( unsigned int r )
    {
    if( r < 1 )
        {
        r = 1;
        }
    _xRes = r;
    _yRes = r;
    rebuild();
    }

void grid::setXRes( unsigned int r )
    {
    if( r < 1 )
        {
        r = 1;
        }
    _xRes = r;
    rebuild();
    }

void grid::setYRes( unsigned int r )
    {
    if( r < 1 )
        {
        r = 1;
        }
    _yRes = r;
    rebuild();
    }

void grid::setColour( math::colour4 colour )
    {
    _colour = colour;
    rebuild();
    }

JFLOAT grid::getXSize( )
    {
    return _xSize;
    }

JFLOAT grid::getYSize( )
    {
    return _ySize;
    }

unsigned int grid::getXRes( )
    {
    return _xRes;
    }

unsigned int grid::getYRes( )
    {
    return _yRes;
    }

math::colour4 grid::setColour( )
    {
    return _colour;
    }

END_JLIB_GRAPHICS_IMPLICIT_NAMESPACE
