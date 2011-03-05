#include "graphics/implicit/graphics.implicit.vector.h"

BEGIN_JLIB_GRAPHICS_IMPLICIT_NAMESPACE

vector::vector( entityG *p, math::colour4 cA, math::colour4 cB, JFLOAT length ) : entityG( p ), _hL( 0 ), line( this ), point( this ), _length( length ), _colourA( cA ), _colourB( cB )
    {
    setDrawable( );
    rebuild( );
    }
void vector::setColourA( math::colour4 cA )
    {
    _colourA = cA;
    }

void vector::setColourB( math::colour4 cB )
    {
    _colourB = cB;
    }

void vector::setLength( JFLOAT length )
    {
    _length = length;
    }

math::colour4 vector::getColourA( )
    {
    return _colourA;
    }

math::colour4 vector::getColourB( )
    {
    return _colourB;
    }

JFLOAT vector::getLength( )
    {
    return _length;
    }

void vector::setHighlightColour( math::colour4 in )
    {
    _hLCol = in;
    }

math::colour4 vector::getHighlightColour( )
    {
    return _hLCol;
    }

bool vector::getHighlight( )
    {
    return _hL;
    }

void vector::setHighlight( bool in )
    {
    _hL = in;
    }

void vector::rebuild( )
    {
    math::colour4 finColA = _colourA, finColB = _colourB;
    line.clear();
    point.clear();

    if( _hL )
        {
        finColA = _hLCol;
        finColB = _hLCol - 0.1;
        }

    line.addLine( line.addPoint( math::triple( 0, 0, 0 ), finColA ), line.addPoint( math::triple( _length, 0, 0 ), finColA ) );

    jLib::vector <unsigned int> triFan;
    triFan.push_back( point.addPoint( math::triple( _length, 0, 0 ), math::triple(), math::vector2(), finColA ) );

    unsigned int res = 6;
    float pointHeight = 0.15;
    float pointRadius = 0.05;
    float angleInc = DEGTORAD * ( 360 / res );
    for( unsigned int x=0; x<res; x++ )
        {
        triFan.push_back( point.addPoint( math::triple( _length - pointHeight, pointRadius * sin( angleInc*x ), pointRadius * cos( angleInc*x ) ), math::triple(), math::vector2(), finColB ) );
        }
    point.addTriangleFan( &(triFan[0]), triFan.size( ) );
    triFan.erase( triFan.begin() );
    point.addPolygon( &(triFan[0]), triFan.size( ) );
    }

END_JLIB_GRAPHICS_IMPLICIT_NAMESPACE
