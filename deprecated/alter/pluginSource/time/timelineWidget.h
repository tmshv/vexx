#ifndef TIMELINEWIDGET_H
#define TIMELINEWIDGET_H

#include "timeGlobal.h"
#include "../SDK/AGlobal"
#include "QWidget"

class TIME_EXPORT timeType
    {
public:
    timeType( )
        { _seconds = 0; }
    explicit timeType( int fr, JFLOAT fps=25 )
        { fromFrames( fr, fps ); }
    timeType( JFLOAT in )
        { fromSeconds( in ); }

    operator JFLOAT( )
        { return _seconds; }

    void fromFrames( int fr, JFLOAT fps=25 )
        { _seconds = (float)fr / (float)fps; }
    void fromSeconds( JFLOAT in )
        { _seconds = in; }

    JFLOAT toSeconds( ) const
        { return _seconds; }
    int toFrames( JFLOAT fps = 25 ) const
        { return (int)ceil( _seconds * fps ); }

private:
    JFLOAT _seconds;
    };

class TIME_EXPORT timelineWidget : public QWidget
    {
    Q_OBJECT
public:
    enum timeRenderMode { Seconds, Frames };
    typedef timeType time;
    jProperty( timeRenderMode, renderMode )
    jProperty( JFLOAT, fps )
    jProperty( time, beginTime )
    jProperty( time, endTime )
    jROProperty( time, currentTime )
    jProperty( time, viewCentre )
    jProperty( time, timeInView )
    jProperty( time, clipBegin )
    jProperty( time, clipEnd )
    jProperty( JFLOAT, majorTimeHeight )
    jProperty( JFLOAT, minorTimeHeight )
public:
    timelineWidget( QWidget *parent=0 );

signals:
    void timeChanged( JFLOAT );

public slots:
    void setTimeSeconds( JFLOAT );
    void setTimeFrames( int );

protected:
    void paintEvent( QPaintEvent * );
    void mousePressEvent( QMouseEvent * );
    void mouseMoveEvent( QMouseEvent * );
    void mouseReleaseEvent( QMouseEvent * );
    void wheelEvent( QWheelEvent * );
    void contextMenuEvent( QContextMenuEvent * );

private:
    QString formatTime( time );
    QPoint _oldMousePos;
    JFLOAT _minTimeSize;
    JFLOAT _maxTimeSize;
    JFLOAT _nowGradientSpread;
    bool _dragActivated;
    bool _zoomActivated;
    };

#endif // TIMELINEWIDGET_H
