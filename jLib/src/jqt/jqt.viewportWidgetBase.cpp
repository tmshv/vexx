#include "jqt/jqt.viewportWidgetBase.h"

namespace jLib
{
namespace jqt
{

viewportWidgetBase::viewportWidgetBase( QWidget *parent, math::colour4 colour, bool mS ) : widget( parent, colour, mS )//, manipulators( rScene )
    {
    timer = new QTimer( parent );
    timer->start( 30 );
    timer->connect( timer, SIGNAL( timeout() ), this, SLOT( refresh() ) );
    setFocusPolicy( Qt::StrongFocus );
    setFocus( Qt::OtherFocusReason );
    }

viewportWidgetBase::~viewportWidgetBase( )
    {
    delete timer;
    }

/*void viewportWidgetBase::mouseMoveEvent( QMouseEvent *event )
    {
    manipulators.mouseEvent(event->x(), event->y(), event->buttons(), ( event->modifiers() & Qt::ShiftModifier ) != 0, ( event->modifiers() & Qt::ControlModifier ) != 0, ( event->modifiers() & Qt::AltModifier ) != 0 );
    }

void viewportWidgetBase::mousePressEvent( QMouseEvent *event )
    {
    manipulators.mouseEvent(event->x(), event->y(), event->buttons(), ( event->modifiers() & Qt::ShiftModifier ) != 0, ( event->modifiers() & Qt::ControlModifier ) != 0, ( event->modifiers() & Qt::AltModifier ) != 0 );
    }

void viewportWidgetBase::mouseReleaseEvent( QMouseEvent *event )
    {
    manipulators.mouseEvent(event->x(), event->y(), event->buttons(), ( event->modifiers() & Qt::ShiftModifier ) != 0, ( event->modifiers() & Qt::ControlModifier ) != 0, ( event->modifiers() & Qt::AltModifier ) != 0 );
    }

void viewportWidgetBase::keyPressEvent( QKeyEvent *event )
    {
    manipulators.keyEvent( event->key(), TRUE, event->isAutoRepeat() );
    }

void viewportWidgetBase::keyReleaseEvent( QKeyEvent *event )
    {
    manipulators.keyEvent( event->key(), FALSE, event->isAutoRepeat() );
    }

void viewportWidgetBase::wheelEvent( QWheelEvent * )
    {
    jDebug<<"No Scroll Events Yet"<<endl;
    }*/

}
}
