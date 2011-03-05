#include "graphics/primitives/graphics.primitives.lineCloud.h"

BEGIN_JLIB_GRAPHICS_PRIMITIVES_NAMESPACE

lineCloud::lineCloud( entity::entityG *p, float w ) : lineArray( p, w ), _usedLines( 0 )
    {
    }

void lineCloud::removeLine( unsigned int x )
    {
    _usedLines--;
    lineArray::removeLine( x );
    }

void lineCloud::removePoint( unsigned int pt )
    {
    for( unsigned int x=0; x<_lineData.size(); x++ )
        {
        if( _lineData[x].a == pt || _lineData[x].b == pt )
            {
            removeLine( x );
            }
        else if( _lineData[x].a > pt )
            {
            _lineData[x].a--;
            }
        else if( _lineData[x].b > pt )
            {
            _lineData[x].b--;
            }
        }
    pointCloud::removePoint( pt );
    }

unsigned int lineCloud::addLine( unsigned int a, unsigned int b )
    {
    if( _usedLines >= sizeLines() )
        {
        setLineArraySize( _usedLines + 1 );
        }
    setLine( _usedLines++, a, b );
    return _usedLines - 1;
    }

void lineCloud::clearLines( )
    {
    _usedLines = 0;
    lineArray::clearLines( );
    }

void lineCloud::clear( )
    {
    clearLines( );
    lineArray::clear( );
    }

unsigned int lineCloud::addLine( math::triple a, math::triple b )
    {
    return addLine( addPoint( a ), addPoint( b ) );
    }

unsigned int lineCloud::addLineLoop( unsigned int *data, unsigned int n )
    {
    if( _usedLines+n+1 >= sizeLines() )
        {
        setLineArraySize( _usedLines+n+1 );
        }

    addLine( data[n-1], data[0] );
    return addLineStrip( data, n );
    }

unsigned int lineCloud::addLineStrip( unsigned int *data, unsigned int n )
    {
    unsigned int ret = _usedLines;
    if( _usedLines+n >= sizeLines() )
        {
        setLineArraySize( _usedLines+n );
        }

    for( unsigned int x=0; x<( n - 1 ); x++ )
        {
        addLine( data[x], data[x+1] );
        }
    return ret;
    }

unsigned int lineCloud::addLineLoop( math::triple *ptData, math::colour4 *colData, unsigned int n )
    {
    unsigned int pts[n];
    if( _usedPoints+n+1 >= sizePoints() )
        {
        setPointArraySize( _usedPoints+n+1 );
        }

    for( unsigned int x=0; x<n; x++ )
        {
        math::colour4 col;
        if( colData != 0 )
            {
            col = colData[x];
            }
        pts[x] = addPoint( ptData[x], col );
        }
    return addLineLoop( pts, n );
    }

unsigned int lineCloud::addLineStrip( math::triple *ptData, math::colour4 *colData, unsigned int n )
    {
    unsigned int pts[n];
    if( _usedPoints+n+1 >= sizePoints() )
        {
        setPointArraySize( _usedPoints+n+1 );
        }

    for( unsigned int x=0; x<n; x++ )
        {
        math::colour4 col;
        if( colData != 0 )
            {
            col = colData[x];
            }
        pts[x] = addPoint( ptData[x], col );
        }
    return addLineStrip( pts, n );
    }

/*unsigned int lineCloud::addBox( JFLOAT width, JFLOAT height, math::point4 origin, math::vector4 across, math::vector4 up )
    {
    unsigned int pts[4];
    across = across.scale( width );
    up = up.scale( height );

    pts[0] = addPoint( origin );
    pts[1] = addPoint( origin + up );
    pts[2] = addPoint( origin + up + across );
    pts[3] = addPoint( origin + across );

    return addLineLoop( pts, 4 );
    }

unsigned int lineCloud::addCube( JFLOAT width, JFLOAT height, JFLOAT depth, math::point4 origin, math::vector4 across, math::vector4 up, math::vector4 in )
    {
    unsigned int ret;
    across = across.scale( width );
    up = up.scale( height );
    in = in.scale( depth );

    ret = addBox( width, height, origin, across, up );
    addBox( width, height, origin + in, across, up );

    for( unsigned int x=1; x<=4; x++ )
        {
        addLine( usedPoints-x, usedPoints-x-4 );
        }

    return ret;
    }*/

END_JLIB_GRAPHICS_PRIMITIVES_NAMESPACE

