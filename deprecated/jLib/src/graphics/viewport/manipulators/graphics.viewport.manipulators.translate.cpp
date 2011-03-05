#include "graphics/viewport/manipulators/graphics.viewport.manipulators.translate.h"
#include "graphics/shape/graphics.shape.sphere.h"
#include "graphics/shape/graphics.shape.line.h"
#include "graphics/shape/graphics.shape.triangle.h"
#include "graphics/shape/graphics.shape.plane.h"

BEGIN_JLIB_GRAPHICS_VIEWPORT_MANIPULATORS_NAMESPACE


translate::translate( )
    {
    addTool( tool );
    }

void translate::onActivateSub()
    {
    workingSet = getManager().getSelectedEntitys( manipulatorManager::entityRef::point );
    if( workingSet.size() > 0 )
        {
        tool.setTransform( workingSet[0]->local );
        }
    else
        {
        tool.setTransform( math::matrix4x4() );
        }
    }

void translate::firstManipulate( tools::base * )
    {
    shape::line currClickLine = scene()->camera()->screenToVector( currentMouse->mouseX, currentMouse->mouseY );
    prevPos = tool.getTranslateDelta();
    }

math::triple translate::currentPosition( )
    {
    return tool.getTransform().translation();
    }

void translate::manipulate( tools::base * )
    {
    shape::line currClickLine = scene()->camera()->screenToVector( currentMouse->mouseX, currentMouse->mouseY );
    math::triple newPos = tool.getTranslateDelta();

    math::triple delta = newPos - prevPos;

    math::matrix4x4 trans = math::matrix4x4::IDENTITY;
    trans.setTranslation( delta );

    tool.setTransform( tool.getTransform() * trans );

    prevPos = newPos;
    prevClickLine = currClickLine;
    }

/*math::triple translate::getNewPos( shape::line currClickLine )
    {
    math::triple newPos( 0.0 );
    if( tool.activeAxis() == implicit::vectorAxis::X )
        {
        shape::line transLi( tool.getTransform().getTranslation(), tool.getTransform().getXAxis() );
        newPos = transLi.sample( transLi.collision( currClickLine ) );
        }
    else if( tool.activeAxis() == implicit::vectorAxis::Y )
        {
        shape::line transLi( tool.getTransform().getTranslation(), tool.getTransform().getYAxis() );
        newPos = transLi.sample( transLi.collision( currClickLine ) );
        }
    else if( tool.activeAxis() == implicit::vectorAxis::Z )
        {
        shape::line transLi( tool.getTransform().getTranslation(), tool.getTransform().getZAxis() );
        newPos = transLi.sample( transLi.collision( currClickLine ) );
        }
    else if( tool.activeAxis() == implicit::vectorAxis::XYZ )
        {
        shape::plane transPl( tool.getTransform().getTranslation(), math::triple( getPass()->getCamera()->facing() ) );

        newPos = currClickLine.sample( transPl.collision( currClickLine ) );
        }
    else
        {
        shape::plane transPl( tool.getTransform().getTranslation(), math::triple( 0.0 ) );

        if( tool.activeAxis() == implicit::vectorAxis::XY )
            {
            transPl.normal = tool.getTransform().getZAxis();
            }
        else if( tool.activeAxis() == implicit::vectorAxis::YZ )
            {
            transPl.normal = tool.getTransform().getXAxis();
            }
        else if( tool.activeAxis() == implicit::vectorAxis::ZX )
            {
            transPl.normal = tool.getTransform().getYAxis();
            }
        newPos = currClickLine.sample( transPl.collision( currClickLine ) );
        }
    return newPos;
    }*/


/*translate::translate( manipulatorManager *p ) : base( p )
    {
    addTool( tool );
    }*/

/*JFLOAT translate::canHandle( renderPass *, const mouseEvent *, const mouseEvent * )
    {
    return 0;
    }

int translate::handle( renderPass *, const mouseEvent *, const mouseEvent * )
    {
    return 0;
    }*/

END_JLIB_GRAPHICS_VIEWPORT_MANIPULATORS_NAMESPACE

#if 0

    if( prev->buttons != FALSE && now->buttons == FALSE )
        {
        _axis.setHighlight( 0 );
        _axis.rebuild();
        return manipulatorManager::PASSIVE || manipulatorManager::UPDATE;
        }
    else if( ( now->buttons & manipulatorManager::LEFT ) != FALSE )
        {
        _axis.setPostTransform( _local );
        float relScale = ( vP->getCamera()->translate - _axis.getPostTransform().getTranslation() ).length() / 4;
        math::triple zero = math::pointToWorldSpace( math::triple( 0, 0, 0 ), _local );
        shape::line xLine( zero, math::vectorToWorldSpace( math::triple( relScale, 0, 0 ), _local ) );
        shape::line yLine( zero, math::vectorToWorldSpace( math::triple( 0, relScale, 0 ), _local ) );
        shape::line zLine( zero, math::vectorToWorldSpace( math::triple( 0, 0, relScale ), _local ) );
        shape::line viewLine = vP->getCamera()->screenToVector( now->mouseX, now->mouseY );

        JFLOAT xColl = xLine.collision( viewLine );
        JFLOAT yColl = yLine.collision( viewLine );
        JFLOAT zColl = zLine.collision( viewLine );

        _axis.setHighlight( 0 );

        bool needsUpdate = FALSE;

        if( shape::sphere( zero, _distance ).shortestDistance( viewLine ) <= 0 )
            {
            _axis.setHighlight( implicit::vectorAxis::XYZ );
            needsUpdate = TRUE;
            }
        else if( viewLine.shortestDistance( xLine ) < _distance && xColl >= 0 && xColl <= 1 )
            {
            _axis.setHighlight( implicit::vectorAxis::X );
            needsUpdate = TRUE;
            }
        else if( viewLine.shortestDistance( yLine ) < _distance && yColl >= 0 && yColl <= 1 )
            {
            _axis.setHighlight( implicit::vectorAxis::Y );
            needsUpdate = TRUE;
            }
        else if( viewLine.shortestDistance( zLine ) < _distance && zColl >= 0 && zColl <= 1 )
            {
            _axis.setHighlight( implicit::vectorAxis::Z );
            needsUpdate = TRUE;
            }
        else
            {
            math::triple xSq = xLine.sample( 0.3 );
            math::triple ySq = yLine.sample( 0.3 );
            math::triple zSq = zLine.sample( 0.3 );

            if( shape::triangle( zero, ySq, xSq ).collision( viewLine ) ||
                    shape::triangle( ySq, ySq + ( xLine.direction * 0.3 ), xSq ).collision( viewLine ) )
                {
                _axis.setHighlight( implicit::vectorAxis::XY );
                needsUpdate = TRUE;
                }
            else if( shape::triangle( zero, xSq, zSq ).collision( viewLine ) ||
                    shape::triangle( xSq, xSq + ( zLine.direction * 0.3 ), zSq ).collision( viewLine ) )
                {
                _axis.setHighlight( implicit::vectorAxis::ZX );
                needsUpdate = TRUE;
                }
            else if( shape::triangle( zero, zSq, ySq ).collision( viewLine ) ||
                    shape::triangle( zSq, zSq + ( yLine.direction * 0.3 ), ySq ).collision( viewLine ) )
                {
                _axis.setHighlight( implicit::vectorAxis::YZ );
                needsUpdate = TRUE;
                }
            }
        _axis.setScale( math::triple( relScale ) );
        vP->addChild( _axis );

        if( needsUpdate )
            {
            _axis.rebuild();
            return manipulatorManager::ACTIVE || manipulatorManager::UPDATE;
            }
        }
#endif
