#include "graphics/viewport/tools/graphics.viewport.tools.base.h"

BEGIN_JLIB_GRAPHICS_VIEWPORT_TOOLS_NAMESPACE

base::base( ) : _canChangeSpace( TRUE ), _isGlobal( FALSE )
    {
    }

base::~base( )
    {
    }

bool base::isEnabled( )
    {
    return _enabled;
    }

entity::entityG *base::scene( )
    {
    assert( _pass );
    return _pass;
    }

void base::setEnabled( bool able )
    {
    _enabled = able;
    }

bool base::isReadOnly( )
    {
    return _readonly;
    }

void base::setReadOnly( bool able )
    {
    _readonly = able;
    }

JFLOAT base::_lineCollTol = 0.05;

void base::setLineTolerance( JFLOAT in )
    {
    _lineCollTol = in;
    }

JFLOAT base::_pointCollTol = 0.1;

void base::setPointTolerance( JFLOAT in )
    {
    _pointCollTol = in;
    }

math::colour4 base::_activeColour = math::colour4( 1, 1, 0 );

void base::setActiveColour( math::colour4 in )
    {
    _activeColour = in;
    }

JFLOAT base::lineTolerance( )
    {
    return _lineCollTol;
    }

JFLOAT base::pointTolerance( )
    {
    return _pointCollTol;
    }

math::colour4 base::getActiveColour( )
    {
    return _activeColour;
    }

JFLOAT base::_manipulatorScale = 0.25;

JFLOAT base::manipulatorScale( )
    {
    return _manipulatorScale;
    }

void base::setManipulatorScale( JFLOAT in )
    {
    _manipulatorScale = in;
    }

void base::setScene( entity::entityG *in )
    {
    _pass = in;
    }

JFLOAT base::distanceToCamera( math::triple in )
    {
    in = _transform.pointMultiply( in );
    return ( _pass->camera()->translate - in ).length();
    }

void base::setClickLine( shape::line newline )
    {
    _previousClickLine = _currentClickLine;
    jAssert( _transform.inverse().multiply( _transform ) == math::matrix4x4::IDENTITY );
    _currentClickLine = shape::lineToSpace( newline, _transform.inverse() );
    }

void base::setTransform( math::matrix4x4 in )
    {
    in.normalise();
    if( getDrawHandle() )
        {
        if( _pass )
            {
            in.setScale( manipulatorScale() /*_pass->getConstantSizePerDistance( ( in.translation() ) )*/ );
            }
        getDrawHandle()->setPostTransform( in );
        }

    _transform = in;
    }

void base::highlight()
    {
    }

void base::setCanChangeSpace( bool able )
    {
    _canChangeSpace = able;
    }

bool base::canChangeSpace()
    {
    return _canChangeSpace;
    }

void base::setTransformSpace( manipulators::base::spaceType space )
    {
    if( canChangeSpace() )
        {
        if( space && !_isGlobal )
            {
            math::matrix4x4 tempMat = getTransform();

            tempMat.normalise();
            _localOrientation = tempMat.orientation();

            _localOrientation = _localOrientation.normalise();

            tempMat.setOrientation( math::quaternion() );

            setTransform( tempMat );
            _isGlobal = TRUE;
            }
        else if( _isGlobal )
            {
            math::matrix4x4 tempMat = getTransform();

            tempMat.setOrientation( _localOrientation );

            setTransform( tempMat );
            _isGlobal = FALSE;
            }
        }
    }

void base::reset()
    {
    }

bool base::isExitButtonState( int n, int p )
    {
    return ( n == FALSE ) && ( p != FALSE );
    }

bool base::isEntryButtonState( int n, int p )
    {
    return ( p == FALSE ) && ( n != FALSE );
    }

math::matrix4x4 base::getTransform( )
    {
    return _transform;
    }

entity::entityD *base::getDrawHandle( )
    {
    return 0;
    }

JFLOAT base::canHandleClickLineAt( )
    {
    return _clickLineHandleDistance;
    }

shape::line base::currentClickLine( )
    {
    return _currentClickLine;
    }

shape::line base::previousClickLine( )
    {
    return _previousClickLine;
    }

void base::calculateClickLineIntersection( )
    {
    setClickLineHandleDistance( floatLimits::nan() );
    }

void base::setClickLineHandleDistance( JFLOAT in )
    {
    _clickLineHandleDistance = in;
    }

END_JLIB_GRAPHICS_VIEWPORT_TOOLS_NAMESPACE
