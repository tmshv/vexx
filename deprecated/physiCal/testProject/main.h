#ifndef MAIN_H
#define MAIN_H

#include "jLib.h"
#include "physiCal.h"
#include <QWidget>

class physiCalTest : public jLib::jqt::viewportWidgetBase
    {
    enum drawType { Points=1, Lines=2, Triangles=4, Collisions=8 };
    jLib::graphics::viewport::manipulatorManager manipulators;
void mouseMoveEvent( QMouseEvent *event )
{ manipulators.mouseEvent(event->x(), event->y(), event->buttons(), ( event->modifiers() & Qt::ShiftModifier ) != 0, ( event->modifiers() & Qt::ControlModifier ) != 0, ( event->modifiers() & Qt::AltModifier ) != 0 ); }
void mousePressEvent( QMouseEvent *event )
    { manipulators.mouseEvent(event->x(), event->y(), event->buttons(), ( event->modifiers() & Qt::ShiftModifier ) != 0, ( event->modifiers() & Qt::ControlModifier ) != 0, ( event->modifiers() & Qt::AltModifier ) != 0 ); }
void mouseReleaseEvent( QMouseEvent *event )
    { manipulators.mouseEvent(event->x(), event->y(), event->buttons(), ( event->modifiers() & Qt::ShiftModifier ) != 0, ( event->modifiers() & Qt::ControlModifier ) != 0, ( event->modifiers() & Qt::AltModifier ) != 0 ); }
void keyPressEvent( QKeyEvent *event )
    {
    if( event->key() == Qt::Key_Tab )
        {
        play = Auto;
        }
    if( event->key() == Qt::Key_Space )
        {
        if( ( event->modifiers()&Qt::ControlModifier ) == FALSE )
            {
            direction = Forwards;
            if( play == Play )
                {
                play = Stop;
                }
            else
                {
                play = Play;
                }
            }
        else
            {
            if( play == Play )
                {
                if( direction == Forwards )
                    {
                    direction = Backwards;
                    }
                else
                    {
                    direction = Forwards;
                    }
                }
            else
                {
                play = Play;
                direction = Backwards;
                }
            }
        }
    else if( event->key() == Qt::Key_1 )
        {
        drawPoints ^= TRUE;
        }
    else if( event->key() == Qt::Key_2 )
        {
        drawLines ^= TRUE;
        }
    else if( event->key() == Qt::Key_3 )
        {
        drawTriangles ^= TRUE;
        }
    else if( event->key() == Qt::Key_4 )
        {
        drawDebug ^= TRUE;
        }
    else if( event->key() == Qt::Key_Up )
        {
        play = Stop;
        drawRefresh = TRUE;
        frameRequest = 0;
        }
    else if( event->key() == Qt::Key_Left )
        {
        play = Stop;
        drawRefresh = TRUE;
        if( frameRequest == 0 )
            {
            frameRequest = sys.framesComplete() - 1;
            }
        else
            {
            frameRequest--;
            }
        }
    else if( event->key() == Qt::Key_Right )
        {
        play = Stop;
        drawRefresh = TRUE;
        frameRequest++;
        if( frameRequest >= sys.framesComplete() )
            {
            frameRequest = 0;
            }
        }
    manipulators.keyEvent( event->key(), TRUE, event->isAutoRepeat() );
    }
void keyReleaseEvent( QKeyEvent *event )
    {
    /*if( event->key() == Qt::Key_Space )
        {
        go = FALSE;
        }*/
    manipulators.keyEvent( event->key(), FALSE, event->isAutoRepeat() );
    }
void wheelEvent( QWheelEvent *event )
    { manipulators.scrollEvent( event->delta() ); }
public:
    enum playMode { Stop, Play, Auto };
    enum playDirection { Forwards, Backwards };
    physiCalTest( QWidget *parent = 0 );
    ~physiCalTest( );
    jLib::graphics::render::scene objectScene;
    cal::system sys;
    jLib::graphics::primitives::lineCloud lines;
    jLib::graphics::primitives::pointCloud points;
    jLib::graphics::primitives::triangleCloud tris;
    playMode play;
    playDirection direction;
    bool drawPoints;
    bool drawLines;
    bool drawTriangles;
    bool drawDebug;
    cal::collisionArray arr;
    bool drawRefresh;
    unsigned int frameRequest;

    void setupGL();
    void refresh();
    };

#endif
