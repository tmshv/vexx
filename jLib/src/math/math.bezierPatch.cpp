#include "math/math.bezierPatch.h"

BEGIN_JLIB_MATH_NAMESPACE

bezierPatch::bezierPatch( unsigned int uDiv, unsigned int vDiv )
    {
    jAssert( uDiv != 0 );
    jAssert( vDiv != 0 );
    setSizeUPoints( uDiv, false );
    setSizeVPoints( vDiv, false );
    rebuildCurves();
    }

triple &bezierPatch::getPoint( unsigned int u, unsigned int v )
    {
    return _uCurves[u].getPoint( v );
    }

void bezierPatch::setSizeVPoints( unsigned int uDiv, bool rebuild )
    {
    jAssert( uDiv != 0 );
    _uDivisions = uDiv+1;
    if( rebuild )
        {
        rebuildCurves();
        }
    }

void bezierPatch::setSizeUPoints( unsigned int vDiv, bool rebuild )
    {
    jAssert( vDiv != 0 );
    _vDivisions = vDiv+1;
    if( rebuild )
        {
        rebuildCurves();
        }
    }

void bezierPatch::rebuildCurves( )
    {
    _uCurves.resize( _uDivisions );
    for( unsigned int u=0; u<_uDivisions; u++ )
        {
        _uCurves[u].resize( _vDivisions );
        }
    }

unsigned int bezierPatch::getSizeUPoints( )
    {
    return _uDivisions;
    }

unsigned int bezierPatch::getSizeVPoints( )
    {
    return _vDivisions;
    }

bezierCurve bezierPatch::getUCurve( unsigned int x )
    {
    jAssert( x < _uCurves.size() );
    return _uCurves[x];
    }

bezierCurve bezierPatch::getVCurve( unsigned int x )
    {
    jAssert( x < _uCurves[0].size() );
    bezierCurve ret;
    for( unsigned int curve=0; curve<_uCurves.size(); curve++ )
        {
        ret.addPoint( _uCurves[curve].getPoint( x ) );
        }
    return ret;
    }

bezierCurve bezierPatch::sample( float uT )
    {
    triple tempPts[_uDivisions];
    for( unsigned int u=0; u<_uDivisions; u++ )
        {
        tempPts[u] = _uCurves[u].sample( uT );
        }
    return bezierCurve( tempPts, _uDivisions );
    }

triple bezierPatch::sample( float uT, float vT )
    {
    return sample( uT ).sample( vT );
    }

vector <vector<triple> > bezierPatch::sampleAlong( unsigned int uRes, unsigned int vRes )
    {
    float vInc = 1.0 / ( vRes - 1.0 );
    float uInc = 1.0 / ( uRes - 1.0 );
    vector <vector<triple> > ret;
    ret.resize( uRes );
    for( unsigned int u=0; u<uRes; u++ )
        {
        float uPos = uInc * u;
        bezierCurve vSection = sample( uPos );
        ret[u].resize( vRes );
        for( unsigned int v=0; v<vRes; v++ )
            {
            ret[u][v] = vSection.sample( vInc * v );
            }
        }
    return ret;
    }

END_JLIB_MATH_NAMESPACE
