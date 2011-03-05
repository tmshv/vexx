#include "nodeEntityManipulator.h"
#include "QPainter"
#include "utils.h"

// operators make manipulations easier further in
QPointF operator*( const QPointF &po, const QSizeF &sc )
    {
    return QPointF( po.x() * sc.width(), po.y() * sc.height() );
    }

QPointF operator/( const QPointF &po1, const QPointF &po2 )
    {
    return QPointF( po1.x() / po2.x(), po1.y() / po2.y() );
    }

QSizeF operator/( const QSizeF &si, const QPointF &po )
    {
    return QSizeF( si.width() / po.x(), si.height() / po.y() );
    }

nodeEntityManipulator::nodeEntityManipulator( QPointF pos, DrawMode draw )
        : _drawMode( draw ), _mode( Translate ), _position( pos ), _angle( 0.0 ), _scale( 1.0, 1.0 )
    {
    }

nodeEntityManipulator::nodeEntityManipulator( qreal x, qreal y, DrawMode draw )
        : _drawMode( draw ), _mode( Scale ), _position( 0.0, 0.0 ), _angle( 0.0 ), _scale( x, y )
    {
    }

nodeEntityManipulator::nodeEntityManipulator( qreal ang, DrawMode draw )
        : _drawMode( draw ), _mode( Rotate ), _position( 0.0, 0.0 ), _angle( ang ), _scale( 1.0, 1.0 )
    {
    }

void nodeEntityManipulator::paint( QPainter *pai )
    {
    // make a pen 1 pixel wide at our current zoom level
    QPen line( Qt::yellow );
    line.setWidthF( 1 / viewScale().x() );
    QBrush fill( Qt::yellow );
    // Set brush colour
        {
        QColor transYellow( Qt::yellow );
        transYellow.setAlphaF( 0.5 );
        fill.setColor( transYellow );
        }
    // if were drawing a full manipulators
    if( drawMode() == Full )
        {
        if( mode() == Translate || mode() == Scale || mode() == Rotate )
            {
            // draw rotate manipulators
            pai->setPen( line );
            if( mode() == Rotate )
                {
                float radius( 150.0 / viewScale().x() );
                float angleDegrees( ( angle() / ( 2.0 * M_PI ) ) * 360.0 );
                pai->drawArc( QRectF( position().x()-radius, position().y()-radius, radius*2, radius*2 ), ( angleDegrees + 4 )*16, 352*16 );
                pai->setBrush( fill );
                pai->drawEllipse( position() + QPointF( radius * cos( angle() ), radius * -sin( angle() ) ), 10/viewScale().x(), 10/viewScale().y() );
                }

            // scale manipulator
            if( mode() == Scale )
                {
                float xSc( -100 * scale().x() );
                float ySc( 100 * scale().y() );
                pai->setBrush( Qt::transparent );
                pai->drawLine( position(), position() + QPointF( xSc+10, 0 ) / viewScale() );
                pai->drawLine( position(), position() + QPointF( 0, ySc-10 ) / viewScale() );
                pai->setBrush( fill );
                pai->drawRect( QRectF( position() + QPointF( xSc, -5 ) / viewScale(), QSize( 10, 10 ) / viewScale() ) );
                pai->drawRect( QRectF( position() + QPointF( -5, ySc-10 ) / viewScale(), QSize( 10, 10 ) / viewScale() ) );
                pai->drawRect( QRectF( position(), position() + scale() * QSizeF( -25, 25 ) / viewScale() ) );
                }

            // scale manipulator
            pai->setBrush( Qt::transparent );
            pai->drawLine( position(), position() + QPointF( 100, 0 ) / viewScale() );
            pai->drawLine( position(), position() + QPointF( 0, -100 ) / viewScale() );
            pai->setBrush( fill );
            pai->drawRect( QRectF( position(), QSizeF( 25, -25 ) / viewScale() ) );
            }
        else
            {
            ALog << "Incorrect Manipulator mode specified";
            }
        }
    else
        {
        // draw little circles for non fill translators
        pai->setPen( line );
        switch( mode() )
            {
            case Translate:
                pai->drawEllipse( position(), 3.0 / viewScale().x(), 3.0 / viewScale().y() ); // draw an ellipse for transform
                break;
            default:
                ALog << "Incorrect Manipulator mode specified";
                break;
            }
        }
    }

bool nodeEntityManipulator::canUse( QPointF point )
    {
    setClickOffset( position() - point );
    if( drawMode() == Full )
        {
        // if mode is translate
        if( mode() == Translate || mode() == Scale || mode() == Rotate )
            {
            // test for rotate manipilator collision
            if( mode() == Rotate )
                {
                qreal dist( findDist( point, position() ) * viewScale().x() );
                if( dist > 145.0 && dist < 155.0 )
                    {
                    setUseRegion( RotateZ );
                    return true;
                    }
                }

            // test for scale manipulator collision
            if( mode() == Scale )
                {
                QRectF scaleRect;

                scaleRect = QRectF( position(), scale() * QSize( -25, 25 ) / viewScale() );
                if( scaleRect.contains( point ) )
                    {
                    setUseRegion( ScaleGeneral );
                    return true;
                    }
                scaleRect = QRectF( position()-(QPointF(0,5)/viewScale()), scale() * QSize( -100, 10 ) / viewScale() );
                if( scaleRect.contains( point ) )
                    {
                    setUseRegion( ScaleX );
                    return true;
                    }
                scaleRect = QRectF( position()-(QPointF(5,0)/viewScale()), scale() * QSize( 10, 100 ) / viewScale() );
                if( scaleRect.contains( point ) )
                    {
                    setUseRegion( ScaleY );
                    return true;
                    }
                }

            // test for translate manipulator collision
            QRectF rect;
            rect = QRectF( position(), QSize( 25, -25 ) / viewScale() );
            if( rect.contains( point ) )
                {
                setUseRegion( TranslateGeneral );
                return true;
                }
            rect = QRectF( position()-(QPointF(0,5)/viewScale()), QSize( 100, 10 ) / viewScale() );
            if( rect.contains( point ) )
                {
                setUseRegion( TranslateX );
                return true;
                }
            rect = QRectF( position()-(QPointF(5,0)/viewScale()), QSize( 10, -100 ) / viewScale() );
            if( rect.contains( point ) )
                {
                setUseRegion( TranslateY );
                return true;
                }
            }
        }
    else if( mode() == Translate )
        {
        // test for collision with mini manipulator
        QRectF manipRect( position()-QPointF(5,5)/viewScale(), QSize( 10, 10 )/viewScale() );

        if( manipRect.contains( point ) )
            {
            setUseRegion( TranslateGeneral );
            return true;
            }
        }
    return false;
    }

void nodeEntityManipulator::use( QPointF nowCoord, QPointF prevCoord )
    {
    switch( useRegion() )
        {
        case TranslateGeneral:
            setPosition( nowCoord + clickOffset() ); // translate manipulator, set its position
            break;
        case TranslateX:
            setPosition( QPointF( nowCoord.x() + clickOffset().x(), position().y() ) ); // translate manipulator, set its position
            break;
        case TranslateY:
            setPosition( QPointF( position().x(), nowCoord.y() + clickOffset().y() ) ); // translate manipulator, set its position
            break;
        case RotateZ:
            {
            QVector2D vecA( nowCoord - position() );
            setAngle( -atan2( vecA.y(), vecA.x() ) );
            break;
            }
        case ScaleGeneral:
            setScale( scale() - ( ( nowCoord - prevCoord ) / QPointF( 150.0, -150.0 ) ) );
            break;
        case ScaleX:
            setScale( QPointF( scale().x() - ( ( nowCoord.x() - prevCoord.x() ) / 150.0 ), scale().y() ) );
            break;
        case ScaleY:
            setScale( QPointF( scale().x(), scale().y() + ( ( nowCoord.y() - prevCoord.y() ) / 150.0 ) ) );
            break;
        }
    }
