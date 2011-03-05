#ifndef JLIB_JQT_VIEWPORTWIDGETBASE_H
#define JLIB_JQT_VIEWPORTWIDGETBASE_H

#include "jqt/jqt.widget.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QWidget>
#include <QTimer>
#include "graphics/viewport/graphics.viewport.manipulatorManager.h"

namespace jLib
{
namespace jqt
{

class JLIB_EXPORT viewportWidgetBase : public jLib::jqt::widget
    {
    Q_OBJECT

public:

    viewportWidgetBase( QWidget *parent = 0, math::colour4 colour=math::colour4( 0.75, 0.75, 0.75 ), bool multiSample=FALSE );
    virtual ~viewportWidgetBase( );
    //jLib::graphics::viewport::manipulatorManager manipulators;
    /*void mouseMoveEvent( QMouseEvent *event );
    void mousePressEvent( QMouseEvent *event );
    void mouseReleaseEvent( QMouseEvent *event );
    void keyPressEvent( QKeyEvent *event );
    void keyReleaseEvent( QKeyEvent *event );
    void wheelEvent( QWheelEvent *event );*/

public slots:
     virtual void refresh() { }
     inline void redraw()
        { updateGL(); }

private:
    QTimer *timer;
    };

}
}

#endif
