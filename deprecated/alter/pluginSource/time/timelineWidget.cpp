#include "timelineWidget.h"
#include "QPainter"
#include "QPalette"
#include "../SDK/ALog"
#include "QMouseEvent"
#include "QApplication"

inline JFLOAT timeToPixel( JFLOAT time, JFLOAT begin, JFLOAT end, JFLOAT width )
    {
    return ( time - begin ) / ( end - begin ) * width;
    }

timelineWidget::timelineWidget( QWidget *p ) : QWidget( p )
    {
    setFocusPolicy( Qt::ClickFocus );
    majorTimeHeight( 20 );
    minorTimeHeight( 10 );
    _minTimeSize = 30.0;
    _maxTimeSize = 60.0;
    _nowGradientSpread = 20;
    _renderMode = Seconds;
    _fps = 25;
    _clipBegin = -1 * jLib::floatLimits::infinity();
    _clipEnd = jLib::floatLimits::infinity();
    _dragActivated = FALSE;
    _zoomActivated = FALSE;
    }

void timelineWidget::mousePressEvent( QMouseEvent *event )
    {
    if( ( event->modifiers()&Qt::AltModifier ) != FALSE )
        {
        _oldMousePos = event->pos();
        if( ( event->buttons()&Qt::LeftButton ) != FALSE )
            {
            _dragActivated = TRUE;
            }
        else if( ( event->buttons()&Qt::RightButton ) != FALSE )
            {
            _zoomActivated = TRUE;
            }
        }
    else
        {
        setTimeSeconds( ( ( (float)event->x() / width() ) * timeInView() ) + ( viewCentre() - ( timeInView() / 2 ) ) );
        emit timeChanged( _currentTime );
        }
    event->accept();
    }

void timelineWidget::contextMenuEvent( QContextMenuEvent *event )
    {
    event->accept();
    }

void timelineWidget::mouseMoveEvent( QMouseEvent *event )
    {
    if( _zoomActivated )
        {
        int dir = ( _oldMousePos.x() - event->pos().x() );
        if( dir > 0 )
            {
            timeInView( timeInView() * 1.03 );
            }
        else
            {
            timeInView( timeInView() / 1.03 );
            }
        _oldMousePos = event->pos();
        update();
        }
    else if( _dragActivated )
        {
        viewCentre( viewCentre() + timeInView() * ( (float)( _oldMousePos.x() - event->pos().x() ) / width() ) );
        update();
        _oldMousePos = event->pos();
        }
    else
        {
        setTimeSeconds( ( ( (float)event->x() / width() ) * timeInView() ) + ( viewCentre() - ( timeInView() / 2 ) ) );
        emit timeChanged( _currentTime );
        }
    }

void timelineWidget::mouseReleaseEvent( QMouseEvent * )
    {
    _dragActivated = FALSE;
    _zoomActivated = FALSE;
    }

void timelineWidget::wheelEvent( QWheelEvent *event )
    {
    timeInView( timeInView() - ( (float)event->delta() / 32.0 ) );
    if( timeInView() > ( endTime() - beginTime() ) )
        {
        timeInView( endTime() - beginTime() );
        }
    update();
    }

void timelineWidget::setTimeSeconds( JFLOAT seconds )
    {
    _currentTime = seconds;

    if( _currentTime < clipBegin() )
        {
        _currentTime = clipBegin();
        }
    if( _currentTime > clipEnd() )
        {
        _currentTime = clipEnd();
        }

    update();
    }

void timelineWidget::setTimeFrames( int in )
    {
    setTimeSeconds( time( in, fps() ) );
    }

void timelineWidget::paintEvent( QPaintEvent * )
    {
    if( jLib::math::fcmp( timeInView( ), 0 ) )
        {
        if( endTime() != jLib::floatLimits::infinity() &&
             beginTime() != jLib::floatLimits::infinity() )
            {
            timeInView( endTime() - beginTime() );
            viewCentre( ( endTime() + beginTime() ) / 2 );
            }
        else
            {
            timeInView( 10 );
            viewCentre( 5 );
            }
        }

    QPainter painter(this);
    painter.setRenderHint( QPainter::Antialiasing, TRUE );

    if( timeInView() > ( endTime( ) - beginTime( ) ) )
        {
        timeInView( endTime( ) - beginTime( ) );
        }

    JFLOAT realBeginDrawing = viewCentre() - ( timeInView()/2 );
    JFLOAT realEndDrawing = viewCentre() + ( timeInView()/2 );

    if( realEndDrawing > endTime() )
        {
        realEndDrawing = endTime( );
        realBeginDrawing = endTime( ) - timeInView( );
        viewCentre( endTime( ) - ( timeInView()/2 ) );
        }
    if( realBeginDrawing < beginTime() )
        {
        realBeginDrawing = beginTime( );
        realEndDrawing = beginTime( ) + timeInView( );
        viewCentre( beginTime( ) + ( timeInView()/2 ) );
        }

    JFLOAT timeSpikeIntervals[] = { 0.125, 0.25, 0.5, 1, 1.25, 2.5, 5, 10, 50 };

    JFLOAT timeSpikeInterval = jLib::floatLimits::nan();

    JFLOAT minDist = jLib::floatLimits::infinity();
    for( int x=0; x<9; x++ )
        {
        JFLOAT size = timeToPixel( timeSpikeIntervals[x], 0, ( realEndDrawing - realBeginDrawing ), width() );

        if( ( size - 50.0 ) < minDist )
            {
            timeSpikeInterval = timeSpikeIntervals[x];
            minDist = size;
            }
        }

    if( timeSpikeInterval != timeSpikeInterval )
        {
        timeSpikeInterval = 1.0;
        }

    JFLOAT beginDrawing = realBeginDrawing - fmod( realBeginDrawing, timeSpikeInterval ) - 1;
    JFLOAT endDrawing = realEndDrawing + ( timeSpikeInterval - fmod( realEndDrawing, timeSpikeInterval ) );

    painter.setPen( QApplication::palette().color( QPalette::Text ) );
    for( JFLOAT x=( beginDrawing - timeSpikeInterval ); x<=endDrawing; x+=timeSpikeInterval )
        {
        JFLOAT actualTime = x + 1;
        JFLOAT xMidPos = timeToPixel( actualTime + ( 0.5 * timeSpikeInterval ), realBeginDrawing, realEndDrawing, width() );
        painter.drawLine( QPointF( xMidPos, height() ), QPointF( xMidPos, height() - minorTimeHeight() ) );

        JFLOAT xPos = timeToPixel( actualTime, realBeginDrawing, realEndDrawing, width() );

        painter.drawText( (int)(xPos+2), (int)(height() - majorTimeHeight()-5), formatTime( actualTime ) );
        painter.drawLine( QPointF( xPos, height() ), QPointF( xPos, height() - majorTimeHeight() ) );
        }

    QColor clipColourA( QApplication::palette().color( QPalette::Shadow ) );
    clipColourA.setAlpha( 200 );
    QColor clipColourB( QApplication::palette().color( QPalette::Shadow ) );
    clipColourB.setAlpha(100 );
    QLinearGradient clipGrad( 0, 0, 0, height() );
    clipGrad.setColorAt( 0.0, clipColourA );
    clipGrad.setColorAt( 1.0, clipColourB );

    QPen clipLine( QApplication::palette().color( QPalette::Shadow ) );
    clipLine.setWidthF( 1.5 );
    if( clipBegin() > realBeginDrawing )
        {
        JFLOAT clipBeginPix = timeToPixel( clipBegin(), realBeginDrawing, realEndDrawing, width() );
        painter.setPen( Qt::transparent );
        painter.setBrush( clipGrad );
        painter.drawRect( 0, 0, (int)clipBeginPix, (int)height() );
        painter.setPen( clipLine );
        painter.drawLine( (int)(clipBeginPix+1), 0, (int)(clipBeginPix+1), height() );
        }

    if( clipEnd() < realEndDrawing )
        {
        JFLOAT clipEndPix = timeToPixel( clipEnd(), realBeginDrawing, realEndDrawing, width() );
        if( clipEndPix < 0 )
            {
            clipEndPix = 0;
            }
        painter.setPen( Qt::transparent );
        painter.setBrush( clipGrad );
        painter.drawRect( (int)clipEndPix, 0, width(), height() );
        painter.setPen( clipLine );
        painter.drawLine( (int)clipEndPix, 0, (int)clipEndPix, height() );
        }


    JFLOAT now = timeToPixel( currentTime(), realBeginDrawing, realEndDrawing, width() );
    QColor nowColour = QApplication::palette().color( QPalette::Highlight );
    nowColour.setAlpha( 200 );

    QPen nowPen( nowColour );
    nowPen.setWidthF( 2.5 );
    painter.setPen( nowPen );

    painter.drawLine( QPointF( now, 0 ), QPointF( now, height() ) );
    painter.setPen( QApplication::palette().color( QPalette::Text ) );
    painter.drawText( (int)(now+2), (int)(height() - majorTimeHeight()-20), formatTime( currentTime() ) );

    nowColour.setAlpha( 150 );
    QLinearGradient highlight( now, 0, now-_nowGradientSpread, 0 );
    highlight.setColorAt(0, nowColour );
    highlight.setColorAt(1, Qt::transparent );
    painter.fillRect( QRectF( now-_nowGradientSpread, 0, _nowGradientSpread, height() ), highlight );
    }

QString timelineWidget::formatTime( time t )
    {
    if( _renderMode == Seconds )
        {
        return QString::number( t.toSeconds(), 'f', 2 ) + "s";
        }
    else
        {
        return QString::number( t.toSeconds(), 'f', 0 );
        }
    }
