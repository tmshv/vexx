#include "timelineWidget.h"
#include "QPainter"
#include "QMouseEvent"
#include "ASettings"
#include "ALog"
#include "APlugin"
#include "math.h"

timelineWidget::timelineWidget( QWidget *parent, APlugin *in ) : QFrame( parent )
    {
    // we want focus
    setFocusPolicy( Qt::StrongFocus );

    // we need an app pointer to to store details
    XGetPropertyMember( app ) = in;

    // set up frame drawing.
    setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Fixed );
    setMinimumSize( 100, 25 );
    setMaximumSize( 2000, 25 );
    setFrameShape( QFrame::Panel );
    setFrameShadow( QFrame::Sunken );

    // not dragging by default
    _dragging = FALSE;

    // connect to signals for updates
    connect( app(), SIGNAL(cleanState()), this, SLOT(refreshDisplayFromArray()) );
    connect( app(), SIGNAL(openState()), this, SLOT(refreshDisplayFromArray()) );

    refreshDisplayFromArray();
    }

void timelineWidget::refreshDisplayFromArray()
    {
    ASettings settings( app(), "timeline" );

    // set values from the array
    _currentFrame = settings.value( "frame", 0 ).toInt();
    _frameSize = settings.value( "frameSize", 10 ).toDouble();
    _fps = settings.value( "fps", 25 ).toDouble();
    _pos = settings.value( "pos", 3 ).toInt();
    _startFrame = settings.value( "startFrame", 0 ).toInt();
    _endFrame = settings.value( "endFrame", 100 ).toInt();

    // update
    update();
    }

void timelineWidget::keyPressEvent( QKeyEvent *event )
    {
    // move the timline left or right on key press
    if( event->key() == Qt::Key_Left )
        {
        ASettings settings( app(), "timeline" );
        settings.setValue( "frame", _currentFrame );

        _currentFrame--;
        if( _currentFrame > _endFrame )
            {
            _currentFrame = _endFrame;
            }
        if( _currentFrame < _startFrame )
            {
            _currentFrame = _startFrame;
            }

        emit currentTimeChanged( _currentFrame );
        update();
        event->accept();
        }
    else if( event->key() == Qt::Key_Right )
        {
        ASettings settings( app(), "timeline" );
        settings.setValue( "frame", _currentFrame );

        _currentFrame++;
        if( _currentFrame > _endFrame )
            {
            _currentFrame = _endFrame;
            }
        if( _currentFrame < _startFrame )
            {
            _currentFrame = _startFrame;
            }

        emit currentTimeChanged( _currentFrame );
        update();
        event->accept();
        }
    else
        {
        QWidget::keyPressEvent( event );
        }
    }

void timelineWidget::paintEvent(QPaintEvent *event)
    {
    QPainter pai( this );

    // nive gradient
    QLinearGradient grad( rect().topLeft(), rect().bottomLeft() );
    grad.setColorAt( 0.0, palette().color( QPalette::Window ) );
    grad.setColorAt( 1.0, palette().color( QPalette::Shadow ) );
    pai.fillRect( rect(), grad );

    // find a nice colour for highlighting
    QColor brCol( palette().color( QPalette::Highlight ) );

    // find the point of the current frame
    float nowTickPos(  _pos + ( _currentFrame * _frameSize ) );

    // draw lines on either side of the frame rect for hot looks
    brCol.setAlpha( 230 );
    pai.setPen( brCol );
    pai.drawLine( nowTickPos, 0, nowTickPos, 25 );
    pai.drawLine( nowTickPos + _frameSize, 0, nowTickPos + _frameSize, 25 );

    // draw a transparent marker for current frame
    brCol.setAlpha( 150 );
    pai.setBrush( brCol );
    pai.setPen( Qt::transparent );
    pai.drawRect( QRect( QPoint( nowTickPos, 0 ), QSize( _frameSize+1, 25 ) ) );

    // draw the long horizontal line for the frames
    pai.setPen( Qt::black );
    pai.setBrush( Qt::transparent );
    pai.drawLine( _pos + ( _startFrame * _frameSize ), 20, _pos + ( ( _endFrame + 1 ) * _frameSize ), 20 );

    // fraw the vertical ticks
    for( int x=_startFrame; x<=(_endFrame+1); x++ )
        {
        float nowPos( _pos + ( x * _frameSize ) );
        int min = 18, max = 20;

        pai.drawLine( nowPos, min, nowPos, max );
        }

    // draw the longer ticks and text for seconds
    for( float x=_startFrame; x<=(_endFrame+1); x+=_fps )
        {
        float nowPos( _pos + ( x * _frameSize ) );
        int min = 13, max = 22;

        pai.drawLine( nowPos, min, nowPos, max );

        QFont fnt( pai.font() );
        fnt.setPixelSize( 10 );
        pai.setFont( fnt );
        pai.drawText( QRect( nowPos-10, 1, 20, 20 ), Qt::TextDontClip|Qt::AlignHCenter, QString::number( x ) );
        }

    // paint the frame
    QFrame::paintEvent( event );
    }

void timelineWidget::contextMenuEvent( QContextMenuEvent *event )
    {
    event->accept();
    }

void timelineWidget::mousePressEvent( QMouseEvent *event )
    {
    // drag of pan depending on mouse state
    if( event->button() == Qt::MidButton || event->button() == Qt::RightButton )
        {
        _dragging = TRUE;
        }
    else if( event->button() == Qt::LeftButton )
        {
        _selecting = TRUE;
        _currentFrame = ( event->pos().x() - _pos ) / _frameSize;

        if( _currentFrame > _endFrame )
            {
            _currentFrame = _endFrame;
            }
        if( _currentFrame < _startFrame )
            {
            _currentFrame = _startFrame;
            }

        update();

        ASettings settings( app(), "timeline" );
        settings.setValue( "frame", _currentFrame );

        emit currentTimeChanged( _currentFrame );
        }
    _oldMousePos = event->pos();
    event->accept();
    }

void timelineWidget::mouseMoveEvent( QMouseEvent *event )
    {
    // apply drag and pan and zoom
    if( _dragging )
        {
        _pos += event->pos().x() - _oldMousePos.x();

        ASettings settings( app(), "timeline" );
        settings.setValue( "pos", _pos );

        update();
        }

    if( _selecting )
        {
        int oldFrame( _currentFrame );
        _currentFrame = ( event->pos().x() - _pos ) / _frameSize;

        if( oldFrame != _currentFrame )
            {
            if( _currentFrame > _endFrame )
                {
                _currentFrame = _endFrame;
                }
            if( _currentFrame < _startFrame )
                {
                _currentFrame = _startFrame;
                }

            update();

            ASettings settings( app(), "timeline" );
            settings.setValue( "frame", _currentFrame );

            emit currentTimeChanged( _currentFrame );
            }
        }

    event->accept();
    _oldMousePos = event->pos();
    }

void timelineWidget::wheelEvent( QWheelEvent *event )
    {
    // wheel zooms in and out
    float frameNow = ( event->pos().x() - _pos ) / _frameSize;

    _frameSize += event->delta() / 50;
    if( _frameSize < 2 )
        {
        _frameSize = 2;
        }
    if( _frameSize > 100 )
        {
        _frameSize = 100;
        }

    _pos = -( ( frameNow * _frameSize ) - event->pos().x() );

    ASettings settings( app(), "timeline" );
    settings.setValue( "pos", _pos );
    settings.setValue( "frameSize", _frameSize );

    update();
    }

void timelineWidget::mouseReleaseEvent( QMouseEvent *event )
    {
    _dragging = FALSE;
    _selecting = FALSE;
    event->accept();
    }

void timelineWidget::setCurrentFrame( int in )
    {
    // set frame, store and redraw
    _currentFrame = in;

    ASettings settings( app(), "timeline" );
    settings.setValue( "frame", _currentFrame );

    update();
    }

void timelineWidget::setMaximumFrame( int in )
    {
    // set max frame, store and redraw
    _endFrame = in;

    ASettings settings( app(), "timeline" );
    settings.setValue( "endFrame", _endFrame );

    update();
    }

void timelineWidget::setMinimumFrame( int in )
    {
    // set min frame, store and redraw
    _startFrame = in;

    ASettings settings( app(), "timeline" );
    settings.setValue( "startFrame", _startFrame );

    update();
    }

void timelineWidget::setFps( double in )
    {
    // set fps, store and redraw
    if( in >= 1.0 )
        {
        _fps = in;

        ASettings settings( app(), "timeline" );
        settings.setValue( "fps", _fps );

        update();
        }
    }

int timelineWidget::currentFrame() const
    {
    return _currentFrame;
    }

int timelineWidget::maximumFrame() const
    {
    return _endFrame;
    }

int timelineWidget::minimumFrame() const
    {
    return _startFrame;
    }

double timelineWidget::fps() const
    {
    return _fps;
    }
