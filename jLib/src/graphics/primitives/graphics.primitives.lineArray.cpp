#include "graphics/primitives/graphics.primitives.lineArray.h"
#include "jLib.glHeader.h"

BEGIN_JLIB_GRAPHICS_PRIMITIVES_NAMESPACE

lineArray::lineArray( entity::entityG *p, float w ) : pointCloud( p, w )
    {
    setDrawable( TRUE );
    }

lineArray::~lineArray( )
    {
    }

void lineArray::removeLine( unsigned int x )
    {
    _lineData.erase( _lineData.begin() + x );
    }

void lineArray::clearLines( )
    {
    _lineData.clear( );
    }

void lineArray::clear( )
    {
    clearLines( );
    pointCloud::clear( );
    }

void lineArray::setLine( unsigned int x, unsigned int a, unsigned int b )
    {
    jAssert( x < _lineData.size() );
    setLineA( x, a );
    setLineB( x, b );
    }

void lineArray::setLineA( unsigned int x, unsigned int a )
    {
    jAssert( x < _lineData.size() );
    _lineData[x].a = a;
    }

void lineArray::setLineB( unsigned int x, unsigned int b )
    {
    jAssert( x < _lineData.size() );
    _lineData[x].b = b;
    }

unsigned int lineArray::getLineA( unsigned int x )
    {
    jAssert( x < _lineData.size() );
    return _lineData[x].a;
    }

unsigned int lineArray::getLineB( unsigned int x )
    {
    jAssert( x < _lineData.size() );
    return _lineData[x].a;
    }

void lineArray::setLineArraySize( unsigned int n )
    {
    _lineData.resize( n );
    }

unsigned int lineArray::sizeLines( )
    {
    return _lineData.size();
    }

void lineArray::draw()
    {
    if( alphaBlend )
        {
        glEnable( GL_BLEND );
        }

#if !POINTARRAY_NEEDS_COLOUR
    colour.load();
#endif
    glLineWidth( _size );


    glEnableClientState( GL_VERTEX_ARRAY );
#if POINTARRAY_NEEDS_COLOUR
    glEnableClientState( GL_COLOR_ARRAY );
#endif

    glVertexPointer( 3, GL_FLOAT, sizeof( struct pointArrayPrim ), &(_pointData[0].position) );
#if POINTARRAY_NEEDS_COLOUR
    glColorPointer( 4, GL_FLOAT, sizeof( struct pointArrayPrim ), &(_pointData[0].colour) );
#endif

    glDrawElements( GL_LINES, _lineData.size()*2, GL_UNSIGNED_INT, &(_lineData[0].a) );


    glDisableClientState( GL_VERTEX_ARRAY );
#if POINTARRAY_NEEDS_COLOUR
    glDisableClientState( GL_COLOR_ARRAY );
#endif

    if( alphaBlend )
        {
        glDisable( GL_BLEND );
        }
    }

#if 0
lineArray::lineArray( entityG *p, float w ) : pointArray( p, w ), _usedIndexs( 0 ), _sizeIndexs( 0 ), _indexBuf( 0 )
    {
    }

lineArray::~lineArray( )
    {
    if( _indexBuf != 0 )
        {
        delete [] _indexBuf;
        }
    }

/*unsigned int lineArray::addLine( unsigned int a, unsigned int b )
    {
    if( a < _numPoints && b < _numPoints )
        {
        unsigned int *temp = new unsigned int[_numIndexs + 2 ];

        for( unsigned int x=0; x<_numIndexs; x++ )
            {
            temp[x] = _indexBuf[x];
            }

        temp[_numIndexs] = a;
        temp[_numIndexs+1] = b;


        _numIndexs += 2;

        if( _indexBuf != 0 )
            {
            delete [] _indexBuf;
            }
        _indexBuf = temp;

        return (_numIndexs/2)-1;
        }
    else
        {
        return _numIndexs;
        }
    }

unsigned int lineCloud::addLine( math::point4 a, math::point4 b, colour4 cA, colour4 cB )
    {
    return addLine( addPoint( a, cA ), addPoint( b, cB ) );
    }

void lineCloud::addLineStrip( vector <unsigned int> pts )
    {
    if( pts.size() > 0 )
        {
        for( unsigned int x=0; x<(pts.size()-1); x++ )
            {
            if( pts[x] < _numPoints )
                {
                addLine( pts[x], pts[x+1] );
                }
            }
        }
    }

void lineCloud::addLineStrip( vector <math::point4> pts )
    {
    vector <colour4> temp;

    for( unsigned int x=0; x<pts.size(); x++ )
        {
        temp.push_back( colour4( ) );
        }

    addLineStrip( pts, temp );
    }

void lineCloud::addLineStrip( vector <math::point4> pts, vector <colour4> col )
    {
    jAssert( pts.size() == col.size() );
    if( pts.size() > 0 )
        {
        vector <unsigned int> temp;

        for( unsigned int x=0; x<pts.size(); x++ )
            {
            temp.push_back( addPoint( pts[x], col[x] ) );
            }

        addLineStrip( temp );
        }
    }

void lineCloud::addLineLoop( vector <unsigned int> pts )
    {
    if( pts.size() > 0 )
        {
        for( unsigned int x=0; x<(pts.size()-1); x++ )
            {
            if( pts[x] < _numPoints )
                {
                addLine( pts[x], pts[x+1] );
                }
            }
        addLine( pts[pts.size()-1], pts[0] );
        }
    }

void lineCloud::addLineLoop( vector <math::point4> pts )
    {
    vector <colour4> temp;

    for( unsigned int x=0; x<pts.size(); x++ )
        {
        temp.push_back( colour4( ) );
        }

    addLineLoop( pts, temp );
    }

void lineCloud::addLineLoop( vector <math::point4> pts, vector <colour4> col )
    {
    jAssert( pts.size() == col.size() );
    if( pts.size() > 2 )
        {
        for( unsigned int x=0; x<pts.size(); x++ )
            {
            if( x == ( pts.size() - 1 ) )
                {
                addLine( pts[x], pts[0], col[x], col[0] );
                }
            else
                {
                addLine( pts[x], pts[x+1], col[x], col[x+1] );
                }
            }
        }
    else if( pts.size() == 2 )
        {
        addLine( pts[0], pts[1], col[0], col[1] );
        }
    }*/

/*void lineCloud::addLineLoop( vector <math::point4> pts )
    {
    if( pts.size() > 2 )
        {
        vector <unsigned int> temp;
        for( unsigned int x=0; x<pts.size(); x++ )
            {
            temp.push_back( addPoint( pts[x] ) );
            }
        temp.push_back( temp[0] );
        addLineStrip( temp );
        }
    else if( pts.size() == 2 )
        {
        addLine( pts[0], pts[1] );
        }
    }*/

void lineArray::removeLine( unsigned int x )
    {
    jAssert( !(_usedIndexs%2) );
    jAssert( !(_sizeIndexs%2) );
    if( x < _usedIndexs && _usedIndexs >= 2 )
        {
        unsigned int *temp = new unsigned int[_usedIndexs - 2 ];

        for( unsigned int a=0; a<(x*2); a++ )
            {
            temp[a] = _indexBuf[a];
            }

        for( unsigned int a=( ( x * 2 ) + 2 ); a<_usedIndexs; a++ )
            {
            temp[a-2] = _indexBuf[a];
            }

        _usedIndexs -= 2;

        if( _indexBuf != 0 )
            {
            delete [] _indexBuf;
            }
        _indexBuf = temp;
        }
    else if( x == 0 && _usedIndexs == 2 )
        {
        _usedIndexs=0;
        delete [] _indexBuf;
        _indexBuf = 0;
        }
    }
void lineArray::setLineArraySize( unsigned int )
    {
    if( _indexBuf != 0 )
        {
        delete [] _indexBuf;
        }
    jDebugNeat<<"I Dont Work..."<<endl;
    }

unsigned int lineArray::sizeLines( )
    {
    return _sizeIndexs;
    }

unsigned int lineArray::usedLines()
    {
    return _usedIndexs/2;
    }

void lineArray::clearLines()
    {
    _usedIndexs=0;
    delete [] _indexBuf;
    _indexBuf = 0;
    }

void lineArray::clear()
    {
    pointArray::clear();
    clearLines();
    }

void lineArray::draw()
    {
    if( alphaBlend )
        {
        glEnable( GL_BLEND );
        }

#if !POINTCLOUD_NEEDS_COLOUR
    colour.load();
#endif
    glLineWidth( _size );


    glEnableClientState( GL_VERTEX_ARRAY );
#if POINTCLOUD_NEEDS_COLOUR
    glEnableClientState( GL_COLOR_ARRAY );
#endif

    glVertexPointer( 3, GL_FLOAT, sizeof( struct pointArrayPrim ), &(_vertexBuf[0].position) );
#if POINTCLOUD_NEEDS_COLOUR
    glColorPointer( 4, GL_FLOAT, sizeof( struct pointArrayPrim ), &(_vertexBuf[0].colour) );
#endif

    glDrawElements( GL_LINES, _usedIndexs, GL_UNSIGNED_INT, _indexBuf );


    glDisableClientState( GL_VERTEX_ARRAY );
#if POINTCLOUD_NEEDS_COLOUR
    glDisableClientState( GL_COLOR_ARRAY );
#endif

    if( alphaBlend )
        {
        glDisable( GL_BLEND );
        }
    }
#endif

END_JLIB_GRAPHICS_PRIMITIVES_NAMESPACE
