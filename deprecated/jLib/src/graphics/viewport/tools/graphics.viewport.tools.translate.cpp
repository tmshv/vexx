#include "graphics/viewport/tools/graphics.viewport.tools.translate.h"
#include "graphics/shape/graphics.shape.line.h"
#include "graphics/shape/graphics.shape.triangle.h"
#include "graphics/shape/graphics.shape.sphere.h"
#include "graphics/shape/graphics.shape.plane.h"

BEGIN_JLIB_GRAPHICS_VIEWPORT_TOOLS_NAMESPACE

translate::translate( ) : _axis( 0 ), _selectedHandle( 0 )
    {
    _axis.setHighlightColour( getActiveColour() );
    _axis.setQuadSize( getQuadSize() );
    _axis.rebuild();
    }

entity::entityD *translate::getDrawHandle()
    {
    return &_axis;
    }

JFLOAT translate::_quadSize = 0.25;

void translate::setQuadSize( JFLOAT in )
    {
    _quadSize = in;
    }

JFLOAT translate::getQuadSize( )
    {
    return _quadSize;
    }

void translate::calculateClickLineIntersection( )
    {
    math::triple zero = math::triple( 0, 0, 0 );
    shape::line xLine( zero, math::triple( 1, 0, 0 ) );
    shape::line yLine( zero, math::triple( 0, 1, 0 ) );
    shape::line zLine( zero, math::triple( 0, 0, 1 ) );

    JFLOAT xColl = xLine.collision( currentClickLine() );
    JFLOAT yColl = yLine.collision( currentClickLine() );
    JFLOAT zColl = zLine.collision( currentClickLine() );

    //jDebugNeat<<"Distance from centre: "<<shape::sphere( zero, pointTolerance() ).shortestDistance( currentClickLine() )<<endl;

    if( shape::sphere( zero, pointTolerance() ).shortestDistance( currentClickLine() ) <= 0 )
        {
        setClickLineHandleDistance( distanceToCamera( zero ) );
        _selectedHandle = implicit::vectorAxis::XYZ;
        return;
        }
    else if( currentClickLine().shortestDistance( xLine ) < lineTolerance() && xColl >= 0 && xColl <= 1 )
        {
        setClickLineHandleDistance( distanceToCamera( zero ) );
        _selectedHandle = implicit::vectorAxis::X;
        return;
        }
    else if( currentClickLine().shortestDistance( yLine ) < lineTolerance() && yColl >= 0 && yColl <= 1 )
        {
        setClickLineHandleDistance( distanceToCamera( zero ) );
        _selectedHandle = implicit::vectorAxis::Y;
        return;
        }
    else if( currentClickLine().shortestDistance( zLine ) < lineTolerance() && zColl >= 0 && zColl <= 1 )
        {
        setClickLineHandleDistance( distanceToCamera( zero ) );
        _selectedHandle = implicit::vectorAxis::Z;
        return;
        }
    else
        {
        math::triple xSq = xLine.sample( 0.3 );
        math::triple ySq = yLine.sample( 0.3 );
        math::triple zSq = zLine.sample( 0.3 );

        if( shape::triangle( zero, ySq, xSq ).collision( currentClickLine() ) ||
                shape::triangle( ySq, ySq + ( xLine.direction * 0.3 ), xSq ).collision( currentClickLine() ) )
            {
            setClickLineHandleDistance( distanceToCamera( zero ) );
            _selectedHandle = implicit::vectorAxis::XY;
            return;
            }
        else if( shape::triangle( zero, xSq, zSq ).collision( currentClickLine() ) ||
                shape::triangle( xSq, xSq + ( zLine.direction * 0.3 ), zSq ).collision( currentClickLine() ) )
            {
            setClickLineHandleDistance( distanceToCamera( zero ) );
            _selectedHandle = implicit::vectorAxis::ZX;
            return;
            }
        else if( shape::triangle( zero, zSq, ySq ).collision( currentClickLine() ) ||
                shape::triangle( zSq, zSq + ( yLine.direction * 0.3 ), ySq ).collision( currentClickLine() ) )
            {
            setClickLineHandleDistance( distanceToCamera( zero ) );
            _selectedHandle = implicit::vectorAxis::YZ;
            return;
            }
        }
    _selectedHandle = 0;
    setClickLineHandleDistance( floatLimits::nan() );
    }

void translate::highlight()
    {
    _axis.setHighlight( _selectedHandle );
    _axis.rebuild();
    }

int translate::activeAxis( )
    {
    return _selectedHandle;
    }

void translate::reset()
    {
    _selectedHandle = 0;
    _axis.setHighlight( 0 );
    _axis.rebuild();
    }


math::triple translate::getTranslateDelta()
    {
    math::triple newPos( 0.0 );
    shape::line currClickLine = lineToSpace( currentClickLine(), getTransform() );
    if( _selectedHandle == implicit::vectorAxis::X )
        {
        shape::line transLi( getTransform().translation(), getTransform().xAxis() );
        newPos = transLi.sample( transLi.collision( currClickLine ) );
        }
    else if( _selectedHandle == implicit::vectorAxis::Y )
        {
        shape::line transLi( getTransform().translation(), getTransform().yAxis() );
        newPos = transLi.sample( transLi.collision( currClickLine ) );
        }
    else if( _selectedHandle == implicit::vectorAxis::Z )
        {
        shape::line transLi( getTransform().translation(), getTransform().zAxis() );
        newPos = transLi.sample( transLi.collision( currClickLine ) );
        }
    else if( _selectedHandle == implicit::vectorAxis::XYZ )
        {
        shape::plane transPl( getTransform().translation(), scene()->camera()->facing() );

        newPos = currClickLine.sample( transPl.collision( currClickLine ) );
        }
    else
        {
        shape::plane transPl( getTransform().translation(), math::triple( 0.0 ) );

        if( _selectedHandle == implicit::vectorAxis::XY )
            {
            transPl.normal = getTransform().zAxis();
            }
        else if( _selectedHandle == implicit::vectorAxis::YZ )
            {
            transPl.normal = getTransform().xAxis();
            }
        else if( _selectedHandle == implicit::vectorAxis::ZX )
            {
            transPl.normal = getTransform().yAxis();
            }
        newPos = currClickLine.sample( transPl.collision( currClickLine ) );
        }
    return newPos;
    }

END_JLIB_GRAPHICS_VIEWPORT_TOOLS_NAMESPACE

