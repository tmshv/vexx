#include "math/math.bezierCurve.h"

BEGIN_JLIB_MATH_NAMESPACE

bezierCurve::bezierCurve()
    {
    }

bezierCurve::bezierCurve( triple *pts, unsigned int size )
    {
    _points.resize( size );

    for( unsigned int x=0; x<size; x++ )
        {
        _points[x] = pts[x];
        }
    }

void bezierCurve::clear()
    {
    _points.clear();
    }

void bezierCurve::addPoint( triple pt )
    {
    _points.push_back( pt );
    }

void bezierCurve::insertPoint( triple pt, unsigned pos )
    {
    jAssert( pos <= _points.size() );
    _points.insert( _points.begin() + pos, triple( pt.ptr() ) );
    }

triple &bezierCurve::getPoint( unsigned int x )
    {
    jAssert( x < _points.size() );
    return _points[x];
    }

triple bezierCurve::sample( float t )
    {
    // Sample the curve at a point on the curve t
    triple ret;
    for( unsigned int i=0; i<_points.size(); i++ )
        {
        unsigned int lastElement = _points.size() - 1;
        ret += ( _points[i] * binomialCoefficient( ( lastElement ), i ) * pow( ( 1 - t ), ( lastElement - i ) ) * pow( t, i ) );
        }
    return ret;
    }

unsigned int bezierCurve::size()
    {
    return _points.size();
    }

void bezierCurve::resize( unsigned int n )
    {
    _points.resize( n );
    }

vector <triple> bezierCurve::sampleAlong( unsigned int divisions )
    {
    vector <triple> ret;
    int divideBy = divisions - 1;
    for( unsigned int x=0; x<divisions; x++ )
        {
        ret.push_back( sample( (float)x / divideBy ) );
        }

    return ret;
    }

END_JLIB_MATH_NAMESPACE

