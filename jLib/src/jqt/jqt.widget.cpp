#include "jqt/jqt.widget.h"

#ifdef QT_OPENGL_LIB

namespace jLib
{
namespace jqt
{

widget::widget( QWidget *parent, math::colour4 colour, bool multisample ) : QGLWidget( getFormat( multisample ), parent ), root( colour )
    {
    }

void widget::initializeGL()
    {
    //rScene.updateContext();
    architecture.updateContext();

    architecture.addScene( "root", root );
    architecture.setRenderable( "root" );

    setupGL();
    }

void widget::setupGL()
    {
    }

void widget::resizeGL( int _w, int _h )
    {
    //rScene.setSize( _w, _h );
    architecture.setSize( _w, _h );
    }

void widget::paintGL()
    {
    //rScene.render();
    architecture.render();
    }

QGLFormat widget::getFormat( bool ms )
    {
    QGLFormat ret;
    ret.setSampleBuffers( ms );
    return ret;
    }

}
}

#endif
